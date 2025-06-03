//Parser.cpp
#include "Parser.hpp"
#include <stack>
#include <stdexcept>
#include <sstream>

Parser::Parser(Lexer& lexer,
               const LR1ParsingTableGenerator& parsingTables,
               const GrammarAugment& grammar,
               std::shared_ptr<SemanticActionProvider> actionProvider)
    : lexer(lexer), parsingTables(parsingTables),
      grammar(grammar), actionProvider(std::move(actionProvider)) {
    buildFlatProductionList();
}

std::string getTerminalName(TokenType type) {
    switch (type) {
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::ID: return "ID";
        case TokenType::STRING: return "STRING";
        case TokenType::BOOL: return "BOOLEAN";
        case TokenType::NULL_VAL: return "NULL_VAL";
        case TokenType::ADD: return "ADD";
        case TokenType::SUB: return "SUB";
        case TokenType::MUL: return "MULT";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::END_OF_FILE: return "$";
        default: return "UNKNOWN";
    }
}


void Parser::buildFlatProductionList() {
    const auto& prods = grammar.getProductions();
    for (const auto& [lhs, rhsList] : prods) {
        for (const auto& rhs : rhsList) {
            flatProductions.emplace_back(lhs, rhs);
        }
    }
}

std::vector<std::shared_ptr<ASTNode>> Parser::parse() {
    std::vector<std::shared_ptr<ASTNode>> results;
    std::stack<int> stateStack;
    std::stack<std::shared_ptr<ASTNode>> valueStack;

    stateStack.push(0);
    auto token = lexer.nextToken();

    while (true) {

        if (token == nullptr || tokenName == "UNKNOWN" || tokenName.empty()) {
            std::ostringstream oss;
            oss << "Lexical error at line " << token->location.line
                << ", column " << token->location.column
                << ": invalid or unrecognized token '" << token->lexeme << "'";
            throw std::runtime_error(oss.str());
        }

        int currentState = stateStack.top();
        std::string tokenName = token->lexeme;

        const auto& actionMap = parsingTables.getActionTable(currentState);
        auto it = actionMap.find(tokenName);
        if (it == actionMap.end()) {
            std::ostringstream oss;
            oss << "Syntax error at line " << token->location.line
                << ", column " << token->location.column
                << ": unexpected token '" << token->lexeme << "'";
            throw std::runtime_error(oss.str());
        }

        const Action& action = it->second;

        switch (action.type) {
        case ActionType::Shift:
            stateStack.push(action.target);
            valueStack.push(std::make_shared<LiteralNode>(
                token->lexeme, "", token->location.line));
            token = lexer.nextToken();
            break;

        case ActionType::Reduce: {
            const auto& [lhs, rhs] = flatProductions[action.target];
            std::vector<std::shared_ptr<ASTNode>> children(rhs.size());
            for (int i = static_cast<int>(rhs.size()) - 1; i >= 0; --i) {
                stateStack.pop();
                children[i] = valueStack.top();
                valueStack.pop();
            }

            int gotoState = parsingTables.getGotoTable(stateStack.top()).at(lhs);
            stateStack.push(gotoState);

            auto resultNode = actionProvider->apply(action.target, children, *token);
            valueStack.push(resultNode);
            break;
        }

        case ActionType::Accept:
            while (!valueStack.empty()) {
                results.insert(results.begin(), valueStack.top());
                valueStack.pop();
            }
            return results;

        case ActionType::Error:
        default:
            throw std::runtime_error("Unexpected error in parser state.");
        }
    }
}

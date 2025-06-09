//ParserDriver.cpp
#include "ParserDriver.hpp"
#include "TokenTypeStringMap.hpp"
#include <iostream>
#include <sstream>

ParserDriver::ParserDriver(const LR1ParsingTableGenerator& tables,
                           SemanticActionDispatcher& dispatcher)
    : parsingTables(tables), semanticDispatcher(dispatcher) {}

ParseResult ParserDriver::parse(const std::vector<std::shared_ptr<Token>>& inputTokens) {
    std::vector<StackEntry> stack;
    stack.push_back({0, nullptr, {0, 0}});

    ParseResult result;
    std::vector<std::shared_ptr<Token>> tokens = inputTokens;

    // Ensure END_OF_FILE is present
    if (tokens.empty() || tokens.back()->type != TokenType::END_OF_FILE) {
        SourceLocation lastLoc = tokens.empty() ? SourceLocation{0,0} : tokens.back()->location;
        tokens.push_back(std::make_shared<Token>(TokenType::END_OF_FILE, "", lastLoc));
    }

    size_t pos = 0;
    while (true) {
        int state = stack.back().state;
        auto token = tokens[pos];
        std::string symbol = token->lexeme.empty() ? tokenTypeToString(token->type) : tokenTypeToString(token->type);

        auto actionMap = parsingTables.getActionTable(state);
        auto it = actionMap.find(symbol);

        if (it == actionMap.end()) {
            reportError(state, token, result.errors);

            // Try error recovery: skip until SEMICOLON
            while (token->type != TokenType::SEMICOLON &&
                   token->type != TokenType::END_OF_FILE &&
                   pos < tokens.size()) {
                ++pos;
                token = tokens[pos];
            }

            if (token->type == TokenType::SEMICOLON) ++pos;
            continue;
        }

        const Action& action = it->second;
        switch (action.type) {
            case ActionType::Shift: {
                stack.push_back({action.target, nullptr, token->location});
                ++pos;
                break;
            }
            case ActionType::Reduce: {
                int prod = action.target;
                int rhsLen = semanticDispatcher.getRHSLength(prod);
                std::string lhs = semanticDispatcher.getLHS(prod);

                if (stack.size() < rhsLen) {
                    std::cerr << "Parser stack underflow: trying to reduce " << lhs
                            << " with rhsLen " << rhsLen << " but stack has only "
                            << stack.size() << std::endl;
                    return result;
                }
                
                std::vector<std::shared_ptr<ASTNode>> children;
                SourceLocation loc;

                for (int i = 0; i < rhsLen; ++i) {
                    if (!stack.empty()) {
                        children.insert(children.begin(), stack.back().node);
                        loc = stack.back().location;
                        stack.pop_back();
                    }
                }

                std::shared_ptr<ASTNode> node = semanticDispatcher.reduce(prod, children, loc);

                // *** FIX: Return immediately if we just reduced the augmented rule ***
                if (lhs == "S`" || lhs == "S'") {
                    if (node != nullptr) {
                        result.ast.push_back(node);
                    }
                    std::cerr << "Se procesó S` \n";
                    return result;
                }

                // Only do GOTO if not the augmented rule!
                if (stack.empty()) {
                    std::cerr << "Parser stack empty after reduction for " << lhs << "\n";
                    return result;
                }

                int prevState = stack.back().state;
                const auto& gotoTable = parsingTables.getGotoTable(prevState);

                auto gotoIt = gotoTable.find(lhs);
                if (gotoIt == gotoTable.end()) {
                    std::cerr << "Invalid GOTO after reduction for " << lhs << "\n";
                    return result;
                }

                stack.push_back({gotoIt->second, node, loc});

                // If top-level rule reduced to a statement, collect it
                if (lhs == "stm" && node != nullptr) {
                    std::cerr << "Se incluyó un stm en la cola \n";
                    result.ast.push_back(node);
                }

                break;
            }
            case ActionType::Accept:
                return result;

            default:
                reportError(state, token, result.errors);
                return result;
        }
    }
}

void ParserDriver::reportError(int state, const std::shared_ptr<Token>& token,
                               std::vector<std::string>& errors) {
    std::ostringstream oss;
    oss << "Syntax error at line " << token->location.line
        << ", column " << token->location.column << ": unexpected token '"
        << token->lexeme << "'";

    const auto& expected = parsingTables.getExpectedTokens(state);
    if (!expected.empty()) {
        oss << ", expected one of: ";
        for (const std::string& sym : expected) oss << sym << " ";
    }

    errors.push_back(oss.str());
    std::cerr << oss.str() << std::endl;
}

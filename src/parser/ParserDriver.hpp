#pragma once

#include "grammar/SemanticActionDispatcher.hpp"
#include "grammar/ParserValue.hpp"
#include "../lexer/.build/Lexer.hpp"
#include "../ast/AST.hpp"
#include "./.build/parsing_tables.hpp"
#include "./.build/grammar_productions.hpp"
#include <vector>
#include <stack>
#include <memory>
#include <string>
#include <variant>
#include <set>

struct ParseResult {
    std::vector<std::shared_ptr<ASTNode>> ast;
    std::vector<std::string> errors;
};

class ParserDriver {
public:
    ParserDriver(SemanticActionDispatcher& dispatcher);
    ParseResult parse(const std::vector<std::shared_ptr<Token>>& tokens);

private:
    SemanticActionDispatcher& dispatcher;
    std::stack<ParserValue> valueStack;  // Can hold both Token and ASTNode
    std::stack<int> stateStack;
    std::vector<std::string> errors;
    size_t currentTokenIndex;

    void reportError(const std::shared_ptr<Token>& token, const std::set<std::string>& expected);
    void skipToNextSemicolon(const std::vector<std::shared_ptr<Token>>& tokens);
    bool isRBRACE(const std::shared_ptr<Token>& token) const;
    bool isSEMICOLON(const std::shared_ptr<Token>& token) const;
    void handleStatementReduction(const std::vector<std::shared_ptr<Token>>& tokens);
    ParseResult handleAccept();
    void handleError(const std::vector<std::shared_ptr<Token>>& tokens);

    const std::map<std::string, Action>& getActionTable(int state) const { return ACTION_TABLE[state]; }
    const std::map<std::string, int>& getGotoTable(int state) const { return GOTO_TABLE[state]; }
};

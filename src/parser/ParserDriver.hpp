#pragma once

#include "core/LR1ParsingTables.hpp"
#include "grammar/SemanticActionDispatcher.hpp"
#include "grammar/ParserValue.hpp"
#include "../lexer/Lexer.hpp"
#include "../ast/AST.hpp"
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
    ParserDriver(const LR1ParsingTableGenerator& tableGen, SemanticActionDispatcher& dispatcher);
    ParseResult parse(const std::vector<std::shared_ptr<Token>>& tokens);

private:
    const LR1ParsingTableGenerator& tableGen;
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

    // Helper functions for ParserValue handling
    bool isNull(const ParserValue& value) const {
        return std::holds_alternative<std::nullptr_t>(value);
    }

    std::string getResultString(const ParserValue& value) const {
        if (isNull(value)) {
            return "null";
        }
        return "Node created";
    }
};

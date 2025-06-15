#pragma once

#include "../ast/AST.hpp"
#include "../lexer/Lexer.hpp"
#include "core/LR1ParsingTables.hpp"
#include "grammar/SemanticActionDispatcher.hpp"
#include <vector>
#include <memory>
#include <string>

struct ParseResult {
    std::vector<std::shared_ptr<ASTNode>> ast;
    std::vector<std::string> errors;
};

class ParserDriver {
public:
    ParserDriver(const LR1ParsingTableGenerator& tables,
                 SemanticActionDispatcher& dispatcher);

    ParseResult parse(const std::vector<std::shared_ptr<Token>>& tokens);

private:
    const LR1ParsingTableGenerator& parsingTables;
    SemanticActionDispatcher& semanticDispatcher;

    struct StackEntry {
        int state;
        std::shared_ptr<ASTNode> node;
        SourceLocation location;
    };

    void reportError(int state, const std::shared_ptr<Token>& token,
                     std::vector<std::string>& errors);
};

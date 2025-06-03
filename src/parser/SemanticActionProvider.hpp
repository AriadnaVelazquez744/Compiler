#pragma once

#include "../ast/AST.hpp"
#include "../lexer/Token.hpp"
#include <vector>
#include <memory>

class SemanticActionProvider {
public:
    virtual ~SemanticActionProvider() = default;

    virtual std::shared_ptr<ASTNode> apply(
        int productionIndex,
        const std::vector<std::shared_ptr<ASTNode>>& children,
        const Token& lookahead
    ) = 0;
};

#pragma once

#include "../../ast/AST.hpp"
#include "../../lexer/Lexer.hpp"
#include <memory>
#include <vector>
#include <variant>

// Define a variant type that can hold any AST-related value
using ParserValue = std::variant<
    std::shared_ptr<Token>,
    std::shared_ptr<ASTNode>,
    std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>>,
    std::shared_ptr<std::vector<Parameter>>,
    std::shared_ptr<std::vector<IfBranch>>,
    std::shared_ptr<std::vector<LetDeclaration>>,
    std::shared_ptr<std::vector<AttributeDeclaration>>,
    std::shared_ptr<std::vector<MethodDeclaration>>,
    std::shared_ptr<TypeBody>,
    std::nullptr_t
>; 
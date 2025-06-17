#pragma once

#include "../../ast/AST.hpp"
#include "../../lexer/Lexer.hpp"
#include "../core/LR1ParsingTables.hpp"
#include "../core/GrammarAugment.hpp"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <variant>

// Structure to store info per production rule
struct RuleInfo {
    std::string lhs;
    int rhsLength;
};

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
    std::shared_ptr<TypeBody>
>;

class SemanticActionDispatcher {
public:
    // Modified constructor to take LR1ParsingTableGenerator
    SemanticActionDispatcher(const LR1ParsingTableGenerator& tableGen);

    // Called by the parser during a reduce step
    std::shared_ptr<ASTNode> reduce(int prodNumber,
                                    const std::vector<ParserValue>& children,
                                    const SourceLocation& location);

    int getRHSLength(int prodNumber) const;
    std::string getLHS(int prodNumber) const;

    // Get the root nodes (statements) that have been processed
    const std::vector<std::shared_ptr<ASTNode>>& getRootNodes() const;
    // Clear the root nodes (useful for resetting between parses)
    void clearRootNodes();

private:
    const LR1ParsingTableGenerator& tableGen;  // Added member
    std::unordered_map<int, RuleInfo> ruleInfo;
    std::vector<int> binaryOpProds;

    // Static vector to store all processed statements
    static std::vector<std::shared_ptr<ASTNode>> rootNodes;

    void initializeRules(); // Fills ruleInfo map
};

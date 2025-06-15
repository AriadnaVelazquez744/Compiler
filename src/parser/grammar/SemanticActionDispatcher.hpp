#pragma once

#include "../../ast/AST.hpp"
#include "../../lexer/Lexer.hpp"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

// Structure to store info per production rule
struct ProductionRule {
    std::string lhs;
    int rhsLength;
    bool isPassThrough = false; // If true, just return children[0]
};

class SemanticActionDispatcher {
public:
    SemanticActionDispatcher();

    // Called by the parser during a reduce step
    std::shared_ptr<ASTNode> reduce(int prodNumber,
                                    const std::vector<std::shared_ptr<ASTNode>>& children,
                                    const SourceLocation& location);

    int getRHSLength(int prodNumber) const;
    std::string getLHS(int prodNumber) const;

private:
    std::unordered_map<int, ProductionRule> ruleInfo;

    std::vector<int> passThroughProds;
    std::vector<int> binaryOpProds;

    void initializeRules(); // Fills ruleInfo map
};

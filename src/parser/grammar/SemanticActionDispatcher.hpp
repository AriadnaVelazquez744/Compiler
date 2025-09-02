#pragma once

#include "../../ast/AST.hpp"
#include "../../lexer/.build/Lexer.hpp"
#include "ParserValue.hpp"
#include "../.build/grammar_productions.hpp"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <iostream>

// Structure to store info per production rule
struct RuleInfo {
    std::string lhs;
    int rhsLength;
};

class SemanticActionDispatcher {
public:
    SemanticActionDispatcher();

    // Called by the parser during a reduce step
    ParserValue reduce(int prodNumber,
                        const std::vector<ParserValue>& children,
                        const SourceLocation& location);

    int getRHSLength(int prodNumber) const;
    std::string getLHS(int prodNumber) const;

    // Get the root nodes (statements) that have been processed
    const std::vector<std::shared_ptr<ASTNode>>& getRootNodes() const;
    // Clear the root nodes (useful for resetting between parses)
    void clearRootNodes();

private:
    std::unordered_map<int, RuleInfo> ruleInfo;
    std::vector<int> binaryOpProds;

    // Static vector to store all processed statements
    static std::vector<std::shared_ptr<ASTNode>> rootNodes;

    void initializeRules(); // Fills ruleInfo map

    // Helper functions for ParserValue handling
    bool isNull(const ParserValue& value) const {
        return std::holds_alternative<std::nullptr_t>(value);
    }

    std::shared_ptr<ASTNode> getASTNode(const ParserValue& value) const {
        if (std::holds_alternative<std::shared_ptr<ASTNode>>(value)) {
            return std::get<std::shared_ptr<ASTNode>>(value);
        }
        return nullptr;
    }

    bool isASTNode(const ParserValue& value) const {
        return std::holds_alternative<std::shared_ptr<ASTNode>>(value);
    }
};

//SemanticActionDispatcher.cpp
#include "SemanticActionDispatcher.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

SemanticActionDispatcher::SemanticActionDispatcher() {
    initializeRules();
}

void SemanticActionDispatcher::initializeRules() {
    // Define production metadata: index → {lhs, rhs length}
    // Index must match the order in your parser's production list!
    ruleInfo[0] = {"S`", 1};                 // S' : program
    ruleInfo[1] = {"program", 1, true};      // program : stm (pass-through)
    ruleInfo[2] = {"alt_gen_0", 1};          // alt_gen_0 : NUMBER
    ruleInfo[3] = {"alt_gen_0", 1};          // alt_gen_0 : ID
    ruleInfo[4] = {"alt_gen_0", 1};          // alt_gen_0 : STRING
    ruleInfo[5] = {"alt_gen_0", 1};          // alt_gen_0 : BOOLEAN
    ruleInfo[6] = {"alt_gen_0", 1};          // alt_gen_0 : NULL_VAL
    ruleInfo[7] = {"alt_gen_0", 1, true};    // alt_gen_0 : elem_expr (pass-through)
    ruleInfo[8] = {"expr", 1, true};         // expr : alt_gen_0 (pass-through)
    ruleInfo[12] = {"stm", 2, true};

    // Binary operator productions: elem_expr : expr ADD expr, etc.
    binaryOpProds = {9, 10, 11}; // Add indices for binary operator productions here
    for (int idx : binaryOpProds) {
        ruleInfo[idx] = {"elem_expr", 3}; // elem_expr : expr ADD expr (and similar for other ops)
    }
}

int SemanticActionDispatcher::getRHSLength(int prodNumber) const {
    auto it = ruleInfo.find(prodNumber);
    return (it != ruleInfo.end()) ? it->second.rhsLength : 0;
}

std::string SemanticActionDispatcher::getLHS(int prodNumber) const {
    auto it = ruleInfo.find(prodNumber);
    return (it != ruleInfo.end()) ? it->second.lhs : "";
}

std::shared_ptr<ASTNode> SemanticActionDispatcher::reduce(int prodNumber,
        const std::vector<std::shared_ptr<ASTNode>>& children,
        const SourceLocation& location) {

    // Don't reduce the augmented rule S' → program
    if (prodNumber == 0) return nullptr;

    // Fast path: pass-through productions like alt_gen_1 : ADD
    auto ruleIt = ruleInfo.find(prodNumber);
    if (ruleIt != ruleInfo.end() && ruleIt->second.isPassThrough) {
        return children[0];
    }

    // Generalized binary operator handling
    if (std::find(binaryOpProds.begin(), binaryOpProds.end(), prodNumber) != binaryOpProds.end()) {
        return std::make_shared<BinaryOpNode>(
            children[1]->type(), // op
            children[0],
            children[2],
            location.line
        );
    }
    
    switch (prodNumber) {
        case 2: // alt_gen_0 : NUMBER
            return std::make_shared<LiteralNode>(children[0]->type(), "number", location.line);

        case 3: // alt_gen_0 : ID
            return std::make_shared<IdentifierNode>(children[0]->type(), location.line);

        case 4: // alt_gen_0 : STRING
            return std::make_shared<LiteralNode>(children[0]->type(), "string", location.line);

        case 5: // alt_gen_0 : BOOLEAN
            return std::make_shared<LiteralNode>(children[0]->type(), "bool", location.line);

        case 6: // alt_gen_0 : NULL_VAL
            return std::make_shared<LiteralNode>("null", "null", location.line);

        default:
            std::cerr << "Unhandled production: " << prodNumber << std::endl;
            return nullptr;
    }
}

//SemanticActionDispatcher.cpp
#include "SemanticActionDispatcher.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

// Initialize the static member
std::vector<std::shared_ptr<ASTNode>> SemanticActionDispatcher::rootNodes;

SemanticActionDispatcher::SemanticActionDispatcher() {
    initializeRules();
}

void SemanticActionDispatcher::initializeRules() {
    // Define production metadata: index → {lhs, rhs length}
    // Index must match the order in your parser's production list!
    ruleInfo[0] = {"S`", 1};                 // S' : program
    ruleInfo[1] = {"program", 1};            // program : stmt
    ruleInfo[2] = {"expr", 1};               // expr : NUMBER
    ruleInfo[3] = {"expr", 1};               // expr : STRING
    ruleInfo[4] = {"expr", 1};               // expr : BOOLEAN
    ruleInfo[5] = {"expr", 1};               // expr : ID
    ruleInfo[6] = {"expr", 1};               // expr : E
    ruleInfo[7] = {"expr", 1};               // expr : PI
    ruleInfo[8] = {"expr", 3};               // expr : expr ADD expr
    ruleInfo[9] = {"program", 1};            // program : stmt
    ruleInfo[10] = {"program", 2};           // program : program stmt
    ruleInfo[11] = {"stmt", 1};              // stmt : expr

    // Binary operator productions
    binaryOpProds = {8}; // Add indices for binary operator productions here

    std::cout << "\n=== Production Rules ===" << std::endl;
    for (const auto& [index, info] : ruleInfo) {
        std::cout << "Rule " << index << ": " << info.lhs << " ::= ";
        if (info.rhsLength == 0) {
            std::cout << "ε";
        } else {
            std::cout << "RHS[" << info.rhsLength << "]";
        }
        std::cout << std::endl;
    }
    std::cout << "=== End Production Rules ===\n" << std::endl;
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
        const std::vector<ParserValue>& children,
        const SourceLocation& location) {

    std::shared_ptr<ASTNode> result;
    std::cout << "\n=== Semantic Action ===" << std::endl;
    std::cout << "Production: " << prodNumber << std::endl;
    std::cout << "Number of children: " << children.size() << std::endl;
    
    switch (prodNumber) {
        case 0: // S' : program
            std::cout << "S' reduction" << std::endl;
            if (std::holds_alternative<std::shared_ptr<ASTNode>>(children[0])) {
                result = std::get<std::shared_ptr<ASTNode>>(children[0]);
                std::cout << "S' reduction: Got program node" << std::endl;
            } else {
                std::cout << "S' reduction: Invalid child type" << std::endl;
            }
            break;

        case 1: // program : stmt
        case 9: // program : stmt (duplicate)
            std::cout << "Program from stmt reduction" << std::endl;
            if (std::holds_alternative<std::shared_ptr<ASTNode>>(children[0])) {
                result = std::get<std::shared_ptr<ASTNode>>(children[0]);
                if (result != nullptr) {
                    rootNodes.push_back(result);
                    std::cout << "Added stmt to root nodes" << std::endl;
                } else {
                    std::cout << "Null stmt node" << std::endl;
                }
            } else {
                std::cout << "Invalid child type in program reduction" << std::endl;
            }
            break;

        case 2: { // expr : NUMBER
            std::cout << "Expression from NUMBER reduction" << std::endl;
            if (std::holds_alternative<std::shared_ptr<Token>>(children[0])) {
                auto token = std::get<std::shared_ptr<Token>>(children[0]);
                result = std::make_shared<LiteralNode>(
                    token->lexeme,
                    "Number",
                    location.line
                );
                std::cout << "Created number literal node: " << token->lexeme << std::endl;
            } else {
                std::cout << "Invalid child type in number reduction" << std::endl;
            }
            break;
        }

        case 3: { // expr : STRING
            std::cout << "Expression from STRING reduction" << std::endl;
            if (std::holds_alternative<std::shared_ptr<Token>>(children[0])) {
                auto token = std::get<std::shared_ptr<Token>>(children[0]);
                result = std::make_shared<LiteralNode>(
                    token->lexeme,
                    "String",
                    location.line
                );
                std::cout << "Created string literal node: " << token->lexeme << std::endl;
            }
            break;
        }

        case 4: { // expr : BOOLEAN
            std::cout << "Expression from BOOLEAN reduction" << std::endl;
            if (std::holds_alternative<std::shared_ptr<Token>>(children[0])) {
                auto token = std::get<std::shared_ptr<Token>>(children[0]);
                result = std::make_shared<LiteralNode>(
                    token->lexeme,
                    "Boolean",
                    location.line
                );
                std::cout << "Created boolean literal node: " << token->lexeme << std::endl;
            }
            break;
        }

        case 5: { // expr : ID
            std::cout << "Expression from ID reduction" << std::endl;
            if (std::holds_alternative<std::shared_ptr<Token>>(children[0])) {
                auto token = std::get<std::shared_ptr<Token>>(children[0]);
                result = std::make_shared<IdentifierNode>(
                    token->lexeme,
                    location.line
                );
                std::cout << "Created identifier node: " << token->lexeme << std::endl;
            }
            break;
        }

        case 6: { // expr : E
            std::cout << "Expression from E reduction" << std::endl;
            if (std::holds_alternative<std::shared_ptr<Token>>(children[0])) {
                auto token = std::get<std::shared_ptr<Token>>(children[0]);
                result = std::make_shared<IdentifierNode>(
                    "e",
                    location.line
                );
                std::cout << "Created identifier node for E" << std::endl;
            }
            break;
        }

        case 7: { // expr : PI
            std::cout << "Expression from PI reduction" << std::endl;
            if (std::holds_alternative<std::shared_ptr<Token>>(children[0])) {
                auto token = std::get<std::shared_ptr<Token>>(children[0]);
                result = std::make_shared<IdentifierNode>(
                    "pi",
                    location.line
                );
                std::cout << "Created identifier node for PI" << std::endl;
            }
            break;
        }

        case 8: { // expr : expr ADD expr
            std::cout << "Binary expression reduction" << std::endl;
            if (std::holds_alternative<std::shared_ptr<ASTNode>>(children[0]) &&
                std::holds_alternative<std::shared_ptr<Token>>(children[1]) &&
                std::holds_alternative<std::shared_ptr<ASTNode>>(children[2])) {
                auto left = std::get<std::shared_ptr<ASTNode>>(children[0]);
                auto op = std::get<std::shared_ptr<Token>>(children[1]);
                auto right = std::get<std::shared_ptr<ASTNode>>(children[2]);
                result = std::make_shared<BinaryOpNode>(
                    op->lexeme,
                    left,
                    right,
                    location.line
                );
                std::cout << "Created binary operation node: " << op->lexeme << std::endl;
            } else {
                std::cout << "Invalid child types in binary expression reduction" << std::endl;
            }
            break;
        }

        case 10: { // program : program stmt
            std::cout << "Program from program stmt reduction" << std::endl;
            if (std::holds_alternative<std::shared_ptr<ASTNode>>(children[0]) &&
                std::holds_alternative<std::shared_ptr<ASTNode>>(children[1])) {
                auto stmt = std::get<std::shared_ptr<ASTNode>>(children[1]);
                if (stmt != nullptr) {
                    rootNodes.push_back(stmt);
                    std::cout << "Added stmt to root nodes in program stmt reduction" << std::endl;
                    result = stmt;
                } else {
                    result = std::get<std::shared_ptr<ASTNode>>(children[0]);
                    std::cout << "Using existing program node" << std::endl;
                }
            } else {
                std::cout << "Invalid child types in program stmt reduction" << std::endl;
            }
            break;
        }

        case 11: // stmt : expr
            std::cout << "Statement from expr reduction" << std::endl;
            if (std::holds_alternative<std::shared_ptr<ASTNode>>(children[0])) {
                result = std::get<std::shared_ptr<ASTNode>>(children[0]);
                std::cout << "Created statement node from expression" << std::endl;
            } else {
                std::cout << "Invalid child type in statement reduction" << std::endl;
            }
            break;

        default:
            std::cerr << "Unhandled production: " << prodNumber << std::endl;
            return nullptr;
    }

    std::cout << "=== End Semantic Action ===" << std::endl;
    return result;
}

// Add method to get root nodes
const std::vector<std::shared_ptr<ASTNode>>& SemanticActionDispatcher::getRootNodes() const {
    return rootNodes;
}

// Add method to clear root nodes
void SemanticActionDispatcher::clearRootNodes() {
    rootNodes.clear();
}

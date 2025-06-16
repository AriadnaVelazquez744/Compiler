// PrecedenceSetup.cpp
#include "PrecedenceSetup.hpp"
#include <string>   // For std::string
#include <vector>   // For std::vector
#include <utility>  // For std::pair
#include <set>      // For std::set
#include <iostream> // For std::cerr

void setupPrecedence(LR1ParsingTableGenerator& tableBuilder) {
    using A = Associativity;

    // Operator precedence and associativity table
    struct PrecInfo { const char* name; int prec; Associativity assoc; };
    
    // Define precedences for terminals
    const PrecInfo ops[] = {
        // Highest precedence first
        {"ADD", 20, A::Left},  // Binary operators have higher precedence than productions
        {"SEMICOLON", 5, A::Left}  // Lowest precedence
    };

    // Set terminal precedence for operators
    for (const auto& op : ops) {
        tableBuilder.setTerminalPrecedence(op.name, op.prec, op.assoc);
    }

    // Define each production separately with its own precedence
    struct ProductionInfo {
        std::string lhs;
        std::vector<std::string> rhs;
        int precedence;
    };

    const std::vector<ProductionInfo> productions = {
        // Basic expressions (highest precedence)
        {"expr", {"NUMBER"}, 25},                    // NUMBER -> expr
        {"expr", {"expr", "ADD", "expr"}, 20},       // expr ADD expr -> expr
        
        // Statements
        {"stmt", {"expr"}, 15},                      // expr -> stmt
        
        // Program
        {"program", {"stmt"}, 10},                    // stmt -> program
        {"program", {"program", "stmt"}, 10},         // program stmt -> program
        
        // Start symbol
        {"S`", {"program"}, 5}                       // program -> S' (lowest precedence)
    };

    // Set precedence for each production
    for (const auto& prod : productions) {
        int prodNum = tableBuilder.getProductionNumber(prod.lhs, prod.rhs);
        if (prodNum != -1) {
            std::cout << "Setting precedence " << prod.precedence 
                      << " for production " << prodNum 
                      << ": " << prod.lhs << " ::= ";
            for (const auto& sym : prod.rhs) {
                std::cout << sym << " ";
            }
            std::cout << std::endl;
            
            tableBuilder.setProductionPrecedence(prodNum, prod.precedence);
        } else {
            std::cerr << "WARNING: Missing production " << prod.lhs << " ::= ";
            for (const auto& sym : prod.rhs) {
                std::cerr << sym << " ";
            }
            std::cerr << "\n";
        }
    }
}
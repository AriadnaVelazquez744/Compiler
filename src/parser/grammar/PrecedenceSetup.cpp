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
        // Lowest number = highest precedence
        {"LPAREN", 0, A::Left},  // Highest precedence for left parenthesis
        {"RPAREN", 0, A::Left},  // Highest precedence for right parenthesis
        {"POW", 1, A::Right},   // Highest precedence, right associative
        {"MUL", 2, A::Left},    // Multiplication and division
        {"DIV", 2, A::Left},
        {"MOD", 2, A::Left},
        {"ADD", 3, A::Left},    // Addition and subtraction
        {"SUB", 3, A::Left},
        {"CONCAT", 4, A::Left}, // String concatenation
        {"CONCAT_SPACE", 4, A::Left},
        {"LT", 5, A::Left},     // Comparison operators
        {"GT", 5, A::Left},
        {"LE", 5, A::Left},
        {"GE", 5, A::Left},
        {"EQ", 5, A::Left},
        {"NE", 5, A::Left},
        {"AND", 6, A::Left},    // Logical operators
        {"OR", 6, A::Left},
        {"SEMICOLON", 7, A::Left}  // Lowest precedence
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
        {"expr", {"NUMBER"}, 0},                     // NUMBER -> expr
        {"expr", {"STRING"}, 0},                     // STRING -> expr
        {"expr", {"BOOL"}, 0},                       // BOOLEAN -> expr
        {"expr", {"ID"}, 0},                         // ID -> expr
        {"expr", {"E"}, 0},                          // E -> expr
        {"expr", {"PI"}, 0},                         // PI -> expr
        {"expr", {"LPAREN", "expr", "RPAREN"}, 0},  // (expr) -> expr (highest precedence)
        
        // Binary operators (ordered by precedence)
        {"expr", {"expr", "POW", "expr"}, 1},        // expr POW expr -> expr
        {"expr", {"expr", "MUL", "expr"}, 2},        // expr MUL expr -> expr
        {"expr", {"expr", "DIV", "expr"}, 2},        // expr DIV expr -> expr
        {"expr", {"expr", "MOD", "expr"}, 2},        // expr MOD expr -> expr
        {"expr", {"expr", "ADD", "expr"}, 3},        // expr ADD expr -> expr
        {"expr", {"expr", "SUB", "expr"}, 3},        // expr SUB expr -> expr
        {"expr", {"expr", "CONCAT", "expr"}, 4},     // expr CONCAT expr -> expr
        {"expr", {"expr", "CONCAT_SPACE", "expr"}, 4}, // expr CONCAT_SPACE expr -> expr
        {"expr", {"expr", "LT", "expr"}, 5},         // expr LT expr -> expr
        {"expr", {"expr", "GT", "expr"}, 5},         // expr GT expr -> expr
        {"expr", {"expr", "LE", "expr"}, 5},         // expr LE expr -> expr
        {"expr", {"expr", "GE", "expr"}, 5},         // expr GE expr -> expr
        {"expr", {"expr", "EQ", "expr"}, 5},         // expr EQ expr -> expr
        {"expr", {"expr", "NE", "expr"}, 5},         // expr NE expr -> expr
        {"expr", {"expr", "AND", "expr"}, 6},        // expr AND expr -> expr
        {"expr", {"expr", "OR", "expr"}, 6},         // expr OR expr -> expr
        
        // Statements
        {"stmt", {"expr"}, 7},                       // expr -> stmt
        
        // Program
        {"program", {"stmt"}, 8},                     // stmt -> program
        {"program", {"program", "stmt"}, 8},          // program stmt -> program
        
        // Start symbol
        {"S`", {"program"}, 9}                      // program -> S' (lowest precedence)
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
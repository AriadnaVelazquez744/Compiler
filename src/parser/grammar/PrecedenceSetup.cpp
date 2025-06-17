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
        {"SIN", 3, A::Right},   // Mathematical functions, between POW and MUL
        {"COS", 3, A::Right},
        {"MIN", 3, A::Right},
        {"MAX", 3, A::Right},
        {"SQRT", 3, A::Right},
        {"LOG", 3, A::Right},
        {"EXP", 3, A::Right},
        {"RANDOM", 3, A::Right},
        {"ADD", 4, A::Left},    // Addition and subtraction
        {"SUB", 4, A::Left},
        {"CONCAT", 5, A::Left}, // String concatenation
        {"CONCAT_SPACE", 5, A::Left},
        {"LT", 6, A::Left},     // Comparison operators
        {"GT", 6, A::Left},
        {"LE", 6, A::Left},
        {"GE", 6, A::Left},
        {"EQ", 6, A::Left},
        {"NE", 6, A::Left},
        {"AND", 7, A::Left},    // Logical operators
        {"OR", 7, A::Left},
        {"NOT", 1, A::Right},   // Unary NOT operator (right associative)
        {"SEMICOLON", 8, A::Left}  // Lowest precedence
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
        
        // Args rules (between basic expressions and unary operators)
        {"args", {}, 11},                            // ε -> args
        {"args", {"expr"}, 10},                      // expr -> args
        {"args", {"args", "COMMA", "expr"}, 10},     // args COMMA expr -> args
        
        // Unary operators
        {"expr", {"SUB", "expr"}, 1},               // -expr -> expr
        {"expr", {"NOT", "expr"}, 1},               // !expr -> expr
        
        // Binary operators (ordered by precedence)
        {"expr", {"expr", "POW", "expr"}, 1},        // expr POW expr -> expr
        {"expr", {"expr", "MUL", "expr"}, 2},        // expr MUL expr -> expr
        {"expr", {"expr", "DIV", "expr"}, 2},        // expr DIV expr -> expr
        {"expr", {"expr", "MOD", "expr"}, 2},        // expr MOD expr -> expr
        
        // Mathematical functions (between POW and MUL)
        {"expr", {"SIN", "LPAREN", "expr", "RPAREN"}, 3},           // SIN(expr) -> expr
        {"expr", {"COS", "LPAREN", "expr", "RPAREN"}, 3},           // COS(expr) -> expr
        {"expr", {"MIN", "LPAREN", "expr", "COMMA", "expr", "RPAREN"}, 3}, // MIN(expr,expr) -> expr
        {"expr", {"MAX", "LPAREN", "expr", "COMMA", "expr", "RPAREN"}, 3}, // MAX(expr,expr) -> expr
        {"expr", {"SQRT", "LPAREN", "expr", "RPAREN"}, 3},          // SQRT(expr) -> expr
        {"expr", {"LOG", "LPAREN", "expr", "COMMA", "expr", "RPAREN"}, 3}, // LOG(expr,expr) -> expr
        {"expr", {"EXP", "LPAREN", "expr", "RPAREN"}, 3},           // EXP(expr) -> expr
        {"expr", {"RANDOM", "LPAREN", "RPAREN"}, 3},                // RANDOM() -> expr
        
        {"expr", {"expr", "ADD", "expr"}, 4},        // expr ADD expr -> expr
        {"expr", {"expr", "SUB", "expr"}, 4},        // expr SUB expr -> expr
        {"expr", {"expr", "CONCAT", "expr"}, 5},     // expr CONCAT expr -> expr
        {"expr", {"expr", "CONCAT_SPACE", "expr"}, 5}, // expr CONCAT_SPACE expr -> expr
        {"expr", {"expr", "LT", "expr"}, 6},         // expr LT expr -> expr
        {"expr", {"expr", "GT", "expr"}, 6},         // expr GT expr -> expr
        {"expr", {"expr", "LE", "expr"}, 6},         // expr LE expr -> expr
        {"expr", {"expr", "GE", "expr"}, 6},         // expr GE expr -> expr
        {"expr", {"expr", "EQ", "expr"}, 6},         // expr EQ expr -> expr
        {"expr", {"expr", "NE", "expr"}, 6},         // expr NE expr -> expr
        {"expr", {"expr", "AND", "expr"}, 7},        // expr AND expr -> expr
        {"expr", {"expr", "OR", "expr"}, 7},         // expr OR expr -> expr
        
        // Statements
        {"stmt", {"expr"}, 8},                       // expr -> stmt
        
        // Program
        {"program", {"stmt"}, 9},                     // stmt -> program
        {"program", {"program", "stmt"}, 9},          // program stmt -> program
        
        // Start symbol
        {"S`", {"program"}, 10}                      // program -> S' (lowest precedence)
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
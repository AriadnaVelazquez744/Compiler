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
    
    const PrecInfo ops[] = {
        // Highest precedence first
        {"POW", 14, A::Right},       // Highest precedence
        {"SUB", 13, A::Right},       // Unary minus
        {"NOT", 13, A::Right},       // Logical NOT
        {"SIN", 12, A::Left}, {"COS", 12, A::Left},  // Functions
        {"MAX", 12, A::Left}, {"MIN", 12, A::Left},
        {"SQRT", 12, A::Left}, {"LOG", 12, A::Left},
        {"EXP", 12, A::Left}, {"RANDOM", 12, A::Left},
        {"MULT", 11, A::Left}, {"DIV", 11, A::Left}, {"MOD", 11, A::Left},
        {"ADD", 10, A::Left}, {"SUB", 10, A::Left},  // Binary subtract
        {"CONCAT", 9, A::Left}, {"CONCAT_SPACE", 9, A::Left},
        {"LT", 8, A::Left}, {"GT", 8, A::Left}, 
        {"LE", 8, A::Left}, {"GE", 8, A::Left},
        {"EQ", 8, A::Left}, {"NE", 8, A::Left},
        {"AND", 7, A::Left},
        {"OR", 6, A::Left},
        {"SEMICOLON", 0, A::Left}  // Lowest precedence
    };

    // Set terminal precedence for all operators
    for (const auto& op : ops) {
        tableBuilder.setTerminalPrecedence(op.name, op.prec, op.assoc);
    }

    // Create static set of valid function names
    static const std::set<std::string> BUILD_IN_FUNCS = {
        "SIN", "COS", "MIN", "MAX", 
        "SQRT", "EXP", "LOG"
    };

    // Set production precedence for each operator-specific
    // (Assumes getProductionNumber returns -1 if not found)
    for (const auto& op : ops) {
        if (op.assoc == A::Right && (op.name == "SUB" || op.name == "NOT")) {
            // Unary operators
            int prod = tableBuilder.getProductionNumber("unary_expr", {op.name, "value"});
            if (prod != -1) tableBuilder.setProductionPrecedence(prod, op.prec);
        }
        else if (BUILD_IN_FUNCS.find(op.name) != BUILD_IN_FUNCS.end()) {
            int prod = tableBuilder.getProductionNumber("build_in", {op.name, "LPAREN", "arg", "RPAREN"});
            if (prod != -1) tableBuilder.setProductionPrecedence(prod, op.prec);
        }    
        else if (op.name == "RANDOM") {
            int prod = tableBuilder.getProductionNumber("build_in", {op.name, "LPAREN", "RPAREN"});
            if (prod != -1) tableBuilder.setProductionPrecedence(prod, op.prec); 
        }
        else if (op.name != "SEMICOLON") {
            int prod = tableBuilder.getProductionNumber("bin_expr", {"value", op.name, "value"});
            if (prod != -1) tableBuilder.setProductionPrecedence(prod, op.prec);
        }
    }

    // Resolve SEMICOLON conflicts
    int prodCond = tableBuilder.getProductionNumber("stm", {"cond"});
    if (prodCond != -1) tableBuilder.setProductionPrecedence(prodCond, 0);  // Lower than SEMICOLON (1)

    // EXPRESSION REDUCTION PRECEDENCE (HIGHER THAN OPERATORS)
    // Update exprReductions to match grammar exactly
    const std::vector<std::pair<std::string, std::vector<std::string>>> exprReductions = {
        {"expr", {"bin_expr"}},
        {"expr", {"unary_expr"}},
        {"expr", {"brace"}},
        {"expr", {"func_call_expr"}},  // Was func_call_expr
        {"expr", {"build_in"}},        // Match grammar production name
        {"expr", {"reassign_expr"}},
        {"expr", {"NUMBER"}},
        {"expr", {"STRING"}},
        {"expr", {"BOOLEAN"}},
        {"expr", {"NULL_VAL"}},
        {"expr", {"ID"}},
        {"expr", {"const"}},
        {"value", {"expr"}},
        {"value", {"cond"}},  // Critical addition
        {"brace", {"LPAREN", "value", "RPAREN"}},
        {"func_call_expr", {"ID", "LPAREN", "arg_opt", "RPAREN"}}  // Match production
    };

    // Add this AFTER setting operator precedences
    for (const auto& [lhs, rhs] : exprReductions) {
        int prodNum = tableBuilder.getProductionNumber(lhs, rhs);
        if (prodNum != -1) {
            tableBuilder.setProductionPrecedence(prodNum, 15);
        } else {
            // Log error for debugging
            std::cerr << "WARNING: Missing production " << lhs << " ::= ";
            for (const auto& sym : rhs) std::cerr << sym << " ";
            std::cerr << "\n";
        }
    }

    // Handle all cond variants
    const std::vector<std::string> condTypes = {"if_expr", "while_expr", "for_expr", "let_expr", "block_expr"};
    for (const auto& type : condTypes) {
        int p = tableBuilder.getProductionNumber("cond", {type});
        if (p != -1) tableBuilder.setProductionPrecedence(p, 15);
    }

}
//SemanticActionDispatcher.cpp
#include "SemanticActionDispatcher.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

// Initialize the static member
std::vector<std::shared_ptr<ASTNode>> SemanticActionDispatcher::rootNodes;

SemanticActionDispatcher::SemanticActionDispatcher(const LR1ParsingTableGenerator& tableGen)
    : tableGen(tableGen) {
    initializeRules();
}

void SemanticActionDispatcher::initializeRules() {
    // Define production metadata: index → {lhs, rhs length}
    // Index must match the order in your parser's production list!
    struct ProductionInfo {
        std::string lhs;
        std::vector<std::string> rhs;
    };

    const std::vector<ProductionInfo> productions = {
        {"S`", {"program"}},                 // S' : program
        {"program", {"stmt"}},               // program : stmt
        {"program", {"program", "stmt"}},    // program : program stmt
        {"args", {}},                        // args : ε
        {"args", {"expr"}},                  // args : expr
        {"args", {"args", "COMMA", "expr"}}, // args : args COMMA expr
        {"expr", {"NUMBER"}},                // expr : NUMBER
        {"expr", {"STRING"}},                // expr : STRING
        {"expr", {"BOOL"}},                  // expr : BOOLEAN
        {"expr", {"ID"}},                    // expr : ID
        {"expr", {"E"}},                     // expr : E
        {"expr", {"PI"}},                    // expr : PI
        {"expr", {"LPAREN", "expr", "RPAREN"}}, // expr : LPAREN expr RPAREN
        {"expr", {"SUB", "expr"}},           // expr : SUB expr
        {"expr", {"NOT", "expr"}},           // expr : NOT expr
        {"expr", {"expr", "ADD", "expr"}},   // expr : expr ADD expr
        {"expr", {"expr", "SUB", "expr"}},   // expr : expr SUB expr
        {"expr", {"expr", "MUL", "expr"}},   // expr : expr MUL expr
        {"expr", {"expr", "DIV", "expr"}},   // expr : expr DIV expr
        {"expr", {"expr", "MOD", "expr"}},   // expr : expr MOD expr
        {"expr", {"expr", "POW", "expr"}},   // expr : expr POW expr
        {"expr", {"expr", "CONCAT", "expr"}}, // expr : expr CONCAT expr
        {"expr", {"expr", "CONCAT_SPACE", "expr"}}, // expr : expr CONCAT_SPACE expr
        {"expr", {"expr", "LT", "expr"}},    // expr : expr LT expr
        {"expr", {"expr", "GT", "expr"}},    // expr : expr GT expr
        {"expr", {"expr", "LE", "expr"}},    // expr : expr LE expr
        {"expr", {"expr", "GE", "expr"}},    // expr : expr GE expr
        {"expr", {"expr", "EQ", "expr"}},    // expr : expr EQ expr
        {"expr", {"expr", "NE", "expr"}},    // expr : expr NE expr
        {"expr", {"expr", "AND", "expr"}},   // expr : expr AND expr
        {"expr", {"expr", "OR", "expr"}},    // expr : expr OR expr
        {"expr", {"SIN", "LPAREN", "expr", "RPAREN"}}, // expr : SIN LPAREN expr RPAREN
        {"expr", {"COS", "LPAREN", "expr", "RPAREN"}}, // expr : COS LPAREN expr RPAREN
        {"expr", {"MIN", "LPAREN", "expr", "COMMA", "expr", "RPAREN"}}, // expr : MIN LPAREN expr COMMA expr RPAREN
        {"expr", {"MAX", "LPAREN", "expr", "COMMA", "expr", "RPAREN"}}, // expr : MAX LPAREN expr COMMA expr RPAREN
        {"expr", {"SQRT", "LPAREN", "expr", "RPAREN"}}, // expr : SQRT LPAREN expr RPAREN
        {"expr", {"LOG", "LPAREN", "expr", "COMMA", "expr", "RPAREN"}}, // expr : LOG LPAREN expr COMMA expr RPAREN
        {"expr", {"EXP", "LPAREN", "expr", "RPAREN"}}, // expr : EXP LPAREN expr RPAREN
        {"expr", {"RANDOM", "LPAREN", "RPAREN"}}, // expr : RANDOM LPAREN RPAREN
        {"stmt", {"expr"}}                   // stmt : expr
    };

    // Initialize ruleInfo using tableBuilder.getProductionNumber
    for (const auto& prod : productions) {
        int prodNum = tableGen.getProductionNumber(prod.lhs, prod.rhs);
        if (prodNum != -1) {
            ruleInfo[prodNum] = {prod.lhs, static_cast<int>(prod.rhs.size())};
            std::cout << "Production " << prodNum << ": " << prod.lhs << " ::= ";
            for (const auto& sym : prod.rhs) {
                std::cout << sym << " ";
            }
            std::cout << std::endl;
        } else {
            std::cerr << "Warning: Production not found: " << prod.lhs << " ::= ";
            for (const auto& sym : prod.rhs) {
                std::cerr << sym << " ";
            }
            std::cerr << std::endl;
        }
    }

    // Binary operator productions
    binaryOpProds = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};

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

ParserValue SemanticActionDispatcher::reduce(int prodNumber,
        const std::vector<ParserValue>& children,
        const SourceLocation& location) {

    ParserValue result;
    std::cout << "\n=== Semantic Action ===" << std::endl;
    std::cout << "Production: " << prodNumber << std::endl;
    std::cout << "Number of children: " << children.size() << std::endl;
    
    // Get production numbers dynamically
    int s_prime_prod = tableGen.getProductionNumber("S'", {"program"});
    int program_stmt_prod = tableGen.getProductionNumber("program", {"stmt"});
    int program_program_stmt_prod = tableGen.getProductionNumber("program", {"program", "stmt"});
    int args_empty_prod = tableGen.getProductionNumber("args", {});
    int args_expr_prod = tableGen.getProductionNumber("args", {"expr"});
    int args_args_comma_expr_prod = tableGen.getProductionNumber("args", {"args", "COMMA", "expr"});
    int expr_number_prod = tableGen.getProductionNumber("expr", {"NUMBER"});
    int expr_string_prod = tableGen.getProductionNumber("expr", {"STRING"});
    int expr_bool_prod = tableGen.getProductionNumber("expr", {"BOOL"});
    int expr_id_prod = tableGen.getProductionNumber("expr", {"ID"});
    int expr_e_prod = tableGen.getProductionNumber("expr", {"E"});
    int expr_pi_prod = tableGen.getProductionNumber("expr", {"PI"});
    int expr_paren_prod = tableGen.getProductionNumber("expr", {"LPAREN", "expr", "RPAREN"});
    int expr_sub_prod = tableGen.getProductionNumber("expr", {"SUB", "expr"});
    int expr_not_prod = tableGen.getProductionNumber("expr", {"NOT", "expr"});
    int expr_add_prod = tableGen.getProductionNumber("expr", {"expr", "ADD", "expr"});
    int expr_sub_binary_prod = tableGen.getProductionNumber("expr", {"expr", "SUB", "expr"});
    int expr_mul_prod = tableGen.getProductionNumber("expr", {"expr", "MUL", "expr"});
    int expr_div_prod = tableGen.getProductionNumber("expr", {"expr", "DIV", "expr"});
    int expr_mod_prod = tableGen.getProductionNumber("expr", {"expr", "MOD", "expr"});
    int expr_pow_prod = tableGen.getProductionNumber("expr", {"expr", "POW", "expr"});
    int expr_concat_prod = tableGen.getProductionNumber("expr", {"expr", "CONCAT", "expr"});
    int expr_concat_space_prod = tableGen.getProductionNumber("expr", {"expr", "CONCAT_SPACE", "expr"});
    int expr_lt_prod = tableGen.getProductionNumber("expr", {"expr", "LT", "expr"});
    int expr_gt_prod = tableGen.getProductionNumber("expr", {"expr", "GT", "expr"});
    int expr_le_prod = tableGen.getProductionNumber("expr", {"expr", "LE", "expr"});
    int expr_ge_prod = tableGen.getProductionNumber("expr", {"expr", "GE", "expr"});
    int expr_eq_prod = tableGen.getProductionNumber("expr", {"expr", "EQ", "expr"});
    int expr_ne_prod = tableGen.getProductionNumber("expr", {"expr", "NE", "expr"});
    int expr_and_prod = tableGen.getProductionNumber("expr", {"expr", "AND", "expr"});
    int expr_or_prod = tableGen.getProductionNumber("expr", {"expr", "OR", "expr"});
    int stmt_expr_prod = tableGen.getProductionNumber("stmt", {"expr"});
    int expr_sin_prod = tableGen.getProductionNumber("expr", {"SIN", "LPAREN", "expr", "RPAREN"});
    int expr_cos_prod = tableGen.getProductionNumber("expr", {"COS", "LPAREN", "expr", "RPAREN"});
    int expr_min_prod = tableGen.getProductionNumber("expr", {"MIN", "LPAREN", "expr", "COMMA", "expr", "RPAREN"});
    int expr_max_prod = tableGen.getProductionNumber("expr", {"MAX", "LPAREN", "expr", "COMMA", "expr", "RPAREN"});
    int expr_sqrt_prod = tableGen.getProductionNumber("expr", {"SQRT", "LPAREN", "expr", "RPAREN"});
    int expr_log_prod = tableGen.getProductionNumber("expr", {"LOG", "LPAREN", "expr", "COMMA", "expr", "RPAREN"});
    int expr_exp_prod = tableGen.getProductionNumber("expr", {"EXP", "LPAREN", "expr", "RPAREN"});
    int expr_random_prod = tableGen.getProductionNumber("expr", {"RANDOM", "LPAREN", "RPAREN"});

    if (prodNumber == s_prime_prod) { // S' : program
        std::cout << "S' reduction" << std::endl;
        if (children.empty()) {
            std::cout << "S' reduction: No children" << std::endl;
            result = nullptr;
        } else if (std::holds_alternative<std::shared_ptr<ASTNode>>(children[0])) {
            result = std::get<std::shared_ptr<ASTNode>>(children[0]);
            std::cout << "S' reduction: Got program node" << std::endl;
        } else {
            std::cout << "S' reduction: Invalid child type" << std::endl;
        }
    }
    else if (prodNumber == program_stmt_prod) { // program : stmt
        std::cout << "Program from stmt reduction" << std::endl;
        if (isASTNode(children[0])) {
            auto node = getASTNode(children[0]);
            if (node != nullptr) {
                rootNodes.push_back(node);
                result = node;
                std::cout << "Added stmt to root nodes" << std::endl;
            } else {
                std::cout << "Null stmt node" << std::endl;
                result = nullptr;
            }
        } else {
            std::cout << "Invalid child type in program reduction" << std::endl;
            result = nullptr;
        }
    }
    else if (prodNumber == program_program_stmt_prod) { // program : program stmt
        std::cout << "Program from program stmt reduction" << std::endl;
        if (isASTNode(children[0]) && isASTNode(children[1])) {
            auto stmt = getASTNode(children[1]);
            if (stmt != nullptr) {
                rootNodes.push_back(stmt);
                std::cout << "Added stmt to root nodes in program stmt reduction" << std::endl;
                result = stmt;
            } else {
                result = getASTNode(children[0]);
                std::cout << "Using existing program node" << std::endl;
            }
        } else {
            std::cout << "Invalid child types in program stmt reduction" << std::endl;
            result = nullptr;
        }
    }
    else if (prodNumber == expr_number_prod) { // expr : NUMBER
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
    }
    else if (prodNumber == expr_string_prod) { // expr : STRING
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
    }
    else if (prodNumber == expr_bool_prod) { // expr : BOOL
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
    }
    else if (prodNumber == expr_id_prod) { // expr : ID
        std::cout << "Expression from ID reduction" << std::endl;
        if (std::holds_alternative<std::shared_ptr<Token>>(children[0])) {
            auto token = std::get<std::shared_ptr<Token>>(children[0]);
            result = std::make_shared<IdentifierNode>(
                token->lexeme,
                location.line
            );
            std::cout << "Created identifier node: " << token->lexeme << std::endl;
        }
    }
    else if (prodNumber == expr_e_prod) { // expr : E
        std::cout << "Expression from E reduction" << std::endl;
        if (std::holds_alternative<std::shared_ptr<Token>>(children[0])) {
            auto token = std::get<std::shared_ptr<Token>>(children[0]);
            result = std::make_shared<IdentifierNode>(
                "e",
                location.line
            );
            std::cout << "Created identifier node for E" << std::endl;
        }
    }
    else if (prodNumber == expr_pi_prod) { // expr : PI
        std::cout << "Expression from PI reduction" << std::endl;
        if (std::holds_alternative<std::shared_ptr<Token>>(children[0])) {
            auto token = std::get<std::shared_ptr<Token>>(children[0]);
            result = std::make_shared<IdentifierNode>(
                "pi",
                location.line
            );
            std::cout << "Created identifier node for PI" << std::endl;
        }
    }
    else if (prodNumber == expr_paren_prod) { // expr : LPAREN expr RPAREN
        std::cout << "Parenthesized expression reduction" << std::endl;
        if (isASTNode(children[1])) {
            result = getASTNode(children[1]);
            std::cout << "Created parenthesized expression node" << std::endl;
        } else {
            std::cout << "Invalid child type in parenthesized expression reduction" << std::endl;
            result = nullptr;
        }
    }
    else if (prodNumber == expr_sub_prod) { // expr : SUB expr
        std::cout << "Unary SUB expression reduction" << std::endl;
        if (std::holds_alternative<std::shared_ptr<Token>>(children[0]) && isASTNode(children[1])) {
            auto op = std::get<std::shared_ptr<Token>>(children[0]);
            auto expr = getASTNode(children[1]);
            if (expr != nullptr) {
                result = std::make_shared<UnaryOpNode>(
                    "-",
                    expr,
                    location.line
                );
                std::cout << "Created unary operation node: -" << std::endl;
            } else {
                result = nullptr;
            }
        } else {
            std::cout << "Invalid child types in unary SUB expression reduction" << std::endl;
            result = nullptr;
        }
    }
    else if (prodNumber == expr_not_prod) { // expr : NOT expr
        std::cout << "Unary NOT expression reduction" << std::endl;
        if (std::holds_alternative<std::shared_ptr<Token>>(children[0]) && isASTNode(children[1])) {
            auto op = std::get<std::shared_ptr<Token>>(children[0]);
            auto expr = getASTNode(children[1]);
            if (expr != nullptr) {
                result = std::make_shared<UnaryOpNode>(
                    "!",
                    expr,
                    location.line
                );
                std::cout << "Created unary operation node: !" << std::endl;
            } else {
                result = nullptr;
            }
        } else {
            std::cout << "Invalid child types in unary NOT expression reduction" << std::endl;
            result = nullptr;
        }
    }
    else if (prodNumber == expr_add_prod || // Binary operations
             prodNumber == expr_sub_binary_prod ||
             prodNumber == expr_mul_prod ||
             prodNumber == expr_div_prod ||
             prodNumber == expr_mod_prod ||
             prodNumber == expr_pow_prod ||
             prodNumber == expr_concat_prod ||
             prodNumber == expr_concat_space_prod ||
             prodNumber == expr_lt_prod ||
             prodNumber == expr_gt_prod ||
             prodNumber == expr_le_prod ||
             prodNumber == expr_ge_prod ||
             prodNumber == expr_eq_prod ||
             prodNumber == expr_ne_prod ||
             prodNumber == expr_and_prod ||
             prodNumber == expr_or_prod) {
        std::cout << "Binary expression reduction" << std::endl;
        if (isASTNode(children[0]) &&
            std::holds_alternative<std::shared_ptr<Token>>(children[1]) &&
            isASTNode(children[2])) {
            auto left = getASTNode(children[0]);
            auto op = std::get<std::shared_ptr<Token>>(children[1]);
            auto right = getASTNode(children[2]);
            if (left != nullptr && right != nullptr) {
                result = std::make_shared<BinaryOpNode>(
                    op->lexeme,
                    left,
                    right,
                    location.line
                );
                std::cout << "Created binary operation node: " << op->lexeme << std::endl;
            } else {
                result = nullptr;
            }
        } else {
            std::cout << "Invalid child types in binary expression reduction" << std::endl;
            result = nullptr;
        }
    }
    else if (prodNumber == stmt_expr_prod) { // stmt : expr
        std::cout << "Statement from expr reduction" << std::endl;
        if (isASTNode(children[0])) {
            result = getASTNode(children[0]);
            std::cout << "Created statement node from expression" << std::endl;
        } else {
            std::cout << "Invalid child type in statement reduction" << std::endl;
            result = nullptr;
        }
    }
    else if (prodNumber == expr_sin_prod) { // expr : SIN LPAREN expr RPAREN
        if (isASTNode(children[2])) {
            auto expr = getASTNode(children[2]);
            if (expr != nullptr) {
                std::vector<std::shared_ptr<ASTNode>> args = {expr};
                result = std::make_shared<BuiltInFunctionNode>("sin", args, location.line);
            } else {
                result = nullptr;
            }
        } else {
            result = nullptr;
        }
    }
    else if (prodNumber == expr_cos_prod) { // expr : COS LPAREN expr RPAREN
        if (std::holds_alternative<std::shared_ptr<ASTNode>>(children[2])) {
            auto expr = std::get<std::shared_ptr<ASTNode>>(children[2]);
            std::vector<std::shared_ptr<ASTNode>> args = {expr};
            result = std::make_shared<BuiltInFunctionNode>("cos", args, location.line);
        }
    }
    else if (prodNumber == expr_min_prod) { // expr : MIN LPAREN expr COMMA expr RPAREN
        if (std::holds_alternative<std::shared_ptr<ASTNode>>(children[2]) &&
            std::holds_alternative<std::shared_ptr<ASTNode>>(children[4])) {
            auto expr1 = std::get<std::shared_ptr<ASTNode>>(children[2]);
            auto expr2 = std::get<std::shared_ptr<ASTNode>>(children[4]);
            std::vector<std::shared_ptr<ASTNode>> args = {expr1, expr2};
            result = std::make_shared<BuiltInFunctionNode>("min", args, location.line);
        }
    }
    else if (prodNumber == expr_max_prod) { // expr : MAX LPAREN expr COMMA expr RPAREN
        if (std::holds_alternative<std::shared_ptr<ASTNode>>(children[2]) &&
            std::holds_alternative<std::shared_ptr<ASTNode>>(children[4])) {
            auto expr1 = std::get<std::shared_ptr<ASTNode>>(children[2]);
            auto expr2 = std::get<std::shared_ptr<ASTNode>>(children[4]);
            std::vector<std::shared_ptr<ASTNode>> args = {expr1, expr2};
            result = std::make_shared<BuiltInFunctionNode>("max", args, location.line);
        }
    }
    else if (prodNumber == expr_sqrt_prod) { // expr : SQRT LPAREN expr RPAREN
        if (std::holds_alternative<std::shared_ptr<ASTNode>>(children[2])) {
            auto expr = std::get<std::shared_ptr<ASTNode>>(children[2]);
            std::vector<std::shared_ptr<ASTNode>> args = {expr};
            result = std::make_shared<BuiltInFunctionNode>("sqrt", args, location.line);
        }
    }
    else if (prodNumber == expr_log_prod) { // expr : LOG LPAREN expr COMMA expr RPAREN
        if (std::holds_alternative<std::shared_ptr<ASTNode>>(children[2]) &&
            std::holds_alternative<std::shared_ptr<ASTNode>>(children[4])) {
            auto expr1 = std::get<std::shared_ptr<ASTNode>>(children[2]);
            auto expr2 = std::get<std::shared_ptr<ASTNode>>(children[4]);
            std::vector<std::shared_ptr<ASTNode>> args = {expr1, expr2};
            result = std::make_shared<BuiltInFunctionNode>("log", args, location.line);
        }
    }
    else if (prodNumber == expr_exp_prod) { // expr : EXP LPAREN expr RPAREN
        if (std::holds_alternative<std::shared_ptr<ASTNode>>(children[2])) {
            auto expr = std::get<std::shared_ptr<ASTNode>>(children[2]);
            std::vector<std::shared_ptr<ASTNode>> args = {expr};
            result = std::make_shared<BuiltInFunctionNode>("exp", args, location.line);
        }
    }
    else if (prodNumber == expr_random_prod) { // expr : RANDOM LPAREN RPAREN
        std::vector<std::shared_ptr<ASTNode>> args;
        result = std::make_shared<BuiltInFunctionNode>("rand", args, location.line);
    }
    else if (prodNumber == args_empty_prod) { // args : ε
        std::cout << "Empty args reduction" << std::endl;
        result = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
        std::cout << "Created empty args vector" << std::endl;
    }
    else if (prodNumber == args_expr_prod) { // args : expr
        std::cout << "Single expr args reduction" << std::endl;
        if (std::holds_alternative<std::shared_ptr<ASTNode>>(children[0])) {
            auto expr = std::get<std::shared_ptr<ASTNode>>(children[0]);
            auto args = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
            args->push_back(expr);
            result = args;
            std::cout << "Created args vector with single expression" << std::endl;
        }
    }
    else if (prodNumber == args_args_comma_expr_prod) { // args : args COMMA expr
        std::cout << "Multiple args reduction" << std::endl;
        if (std::holds_alternative<std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>>>(children[0]) &&
            std::holds_alternative<std::shared_ptr<ASTNode>>(children[2])) {
            auto existing_args = std::get<std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>>>(children[0]);
            auto new_expr = std::get<std::shared_ptr<ASTNode>>(children[2]);
            existing_args->push_back(new_expr);
            result = existing_args;
            std::cout << "Added expression to existing args vector" << std::endl;
        }
    }
    else {
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

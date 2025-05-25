#include "Parser.hpp"
#include <iostream>
#include <stdexcept>
#include <unordered_map>

Parser::Parser(std::shared_ptr<Lexer> lexer)
    : lexer(std::move(lexer)) {
    advance(); // load first token
}

const std::vector<ParserError>& Parser::getErrors() const {
    return errors;
}

// Advance to next token
void Parser::advance() {
    currentToken = lexer->nextToken();
}

// Match a specific token type (and advance if matched)
bool Parser::match(TokenType type) {
    if (currentToken->type == type) {
        advance();
        return true;
    }
    return false;
}

// Expect a token or report a syntax error
void Parser::expect(TokenType type, const std::string& expectedMessage) {
    if (!match(type)) {
        errors.emplace_back(
            "Se esperaba '" + expectedMessage + "'",
            currentToken->location,
            expectedMessage
        );
    }
}


std::shared_ptr<ASTNode> Parser::parseStatement() {
    // Handle "print(...);" form
    if (match(TokenType::PRINT)) {
        expect(TokenType::LPAREN, "(");
        auto arg = parseExpression();
        expect(TokenType::RPAREN, ")");
        expect(TokenType::SEMICOLON, ";");

        std::vector<std::shared_ptr<ASTNode>> args;
        if (arg) args.push_back(arg);

        return std::make_shared<BuiltInFunctionNode>("print", args, currentToken->location.line);
    }

    // Block statement: { ... }
    if (currentToken->type == TokenType::LBRACE) {
        return parseBlockExpr();
    }

    // Function declaration: FUNC ID (...) => body or block
    if (match(TokenType::FUNC)) {
        if (currentToken->type == TokenType::ID) {
            std::string funcName = currentToken->lexeme;
            advance();

            expect(TokenType::LPAREN, "(");
            auto params = parseParams();
            expect(TokenType::RPAREN, ")");

            // Inline function: uses LAMBDA
            if (match(TokenType::LAMBDA)) { // LAMBDA is => in your grammar
                auto body = parseBody();
                return std::make_shared<FunctionDeclarationNode>(funcName, std::make_shared<std::vector<Parameter>>(params), body, true, currentToken->location.line);
            }
            // Block function
            else {
                auto body = parseBlockExpr();
                return std::make_shared<FunctionDeclarationNode>(funcName, std::make_shared<std::vector<Parameter>>(params), body, false, currentToken->location.line);
            }
        } else {
            errors.emplace_back(
                "Se esperaba un nombre de función después de 'function'",
                currentToken->location,
                "ID"
            );
            return nullptr;
        }
    }

    // Let, while, for
    if (currentToken->type == TokenType::LET) return parseLetExpr();
    if (currentToken->type == TokenType::WHILE) return parseWhileExpr();
    if (currentToken->type == TokenType::FOR) return parseForExpr();

    // General expression
    auto expr = parseExpression();
    expect(TokenType::SEMICOLON, ";");
    return expr;
}

std::shared_ptr<ASTNode> Parser::parseExpression() {
    TokenType type = currentToken->type;
    SourceLocation loc = currentToken->location;

    // --- Handle simple literals and identifiers
    if (type == TokenType::NUMBER) {
        std::string value = currentToken->lexeme;
        advance();
        return std::make_shared<LiteralNode>(value, "Number", loc.line);
    }

    if (type == TokenType::STRING) {
        std::string value = currentToken->lexeme;
        advance();
        return std::make_shared<LiteralNode>(value, "String", loc.line);
    }

    if (type == TokenType::BOOL) {
        std::string value = (currentToken->lexeme == "True") ? "true" : "false";
        advance();
        return std::make_shared<LiteralNode>(value, "Boolean", loc.line);
    }

    if (type == TokenType::NULL_VAL) {
        advance();
        return std::make_shared<LiteralNode>("null", "Null", loc.line);
    }

    if (type == TokenType::ID) {
        // Could be an identifier, function call, or assignment
        std::string idName = currentToken->lexeme;
        advance();

        // Lookahead: function call
        if (currentToken->type == TokenType::LPAREN) {
            advance();  // consume '('
            auto args = parseArgs();
            expect(TokenType::RPAREN, ")");
            return std::make_shared<FunctionCallNode>(idName, args, loc.line);
        }

        // Lookahead: reassignment := 
        if (currentToken->type == TokenType::REASSIGN) {
            advance();
            auto rhs = parseExpression();
            return std::make_shared<AssignmentNode>(idName, rhs, loc.line);
        }

        // Just an identifier
        return std::make_shared<IdentifierNode>(idName, loc.line);
    }

    // --- Parse control structures
    if (type == TokenType::IF) return parseIfExpr();
    if (type == TokenType::WHILE) return parseWhileExpr();
    if (type == TokenType::FOR) return parseForExpr();
    if (type == TokenType::LET) return parseLetExpr();

    // --- Parse grouped block
    if (type == TokenType::LBRACE) return parseBlockExpr();

    // --- Fallback to element expression (recursive precedence parser)
    return parseElemExpr();
}

std::shared_ptr<ASTNode> Parser::parseBlockExpr() {
    expect(TokenType::LBRACE, "{");
    auto stmts = parseBlockBody();
    expect(TokenType::RBRACE, "}");

    return std::make_shared<BlockNode>(stmts, currentToken->location.line);
}

std::vector<std::shared_ptr<ASTNode>> Parser::parseBlockBody() {
    std::vector<std::shared_ptr<ASTNode>> body;

    while (currentToken->type != TokenType::RBRACE &&
           currentToken->type != TokenType::END_OF_FILE) {
        auto stmt = parseStatement();
        if (stmt) body.push_back(stmt);
    }

    return body;
}

int getPrecedence(TokenType type) {
    switch (type) {
        case TokenType::NOT: return 90; // unary
        case TokenType::POW:
        case TokenType::SIN:
        case TokenType::COS:
        case TokenType::EXP:
        case TokenType::SQRT:
        case TokenType::LOG:
        case TokenType::MIN:
        case TokenType::MAX:
        case TokenType::RANDOM: return 80;

        case TokenType::MUL:
        case TokenType::DIV:
        case TokenType::MOD: return 70;

        case TokenType::ADD:
        case TokenType::SUB: return 60;

        case TokenType::CONCAT:
        case TokenType::CONCAT_SPACE: return 55;

        case TokenType::LT:
        case TokenType::GT:
        case TokenType::LE:
        case TokenType::GE:
        case TokenType::EQ:
        case TokenType::NE: return 50;

        case TokenType::AND:
        case TokenType::OR: return 40;

        default: return -1;
    }
}

std::shared_ptr<ASTNode> Parser::parseElemExpr(std::shared_ptr<ASTNode> left, int minPrec) {
    // ---- Initial expression
    if (!left) {
        TokenType type = currentToken->type;
        SourceLocation loc = currentToken->location;

        // Unary operators
        if (match(TokenType::SUB)) {
            auto operand = parseExpression();  // allows complex sub-expr like `-if (x) ...`
            if (!operand) {
                errors.emplace_back("Se esperaba una expresión después del operador '-'", currentToken->location, "expresión");
                return nullptr;
            }
            return std::make_shared<UnaryOpNode>("-", operand, loc.line);
        }

        if (match(TokenType::NOT)) {
            auto operand = parseExpression();
            if (!operand) {
                errors.emplace_back("Se esperaba una expresión después del operador '!'", currentToken->location, "expresión");
                return nullptr;
            }
            return std::make_shared<UnaryOpNode>("!", operand, loc.line);
        }

        // Grouped expression
        if (match(TokenType::LPAREN)) {
            auto expr = parseExpression();
            if (!match(TokenType::RPAREN)) {
                errors.emplace_back("Falta ')' para cerrar la expresión entre paréntesis", currentToken->location, ")");
            }
            return expr;
        }

        // Constants
        if (match(TokenType::PI)) return std::make_shared<IdentifierNode>("pi", loc.line);
        if (match(TokenType::E))  return std::make_shared<IdentifierNode>("e", loc.line);

        // Built-in functions
        const std::unordered_map<TokenType, std::pair<std::string, int>> builtins = {
            {TokenType::SIN, {"sin", 1}}, {TokenType::COS, {"cos", 1}},
            {TokenType::SQRT, {"sqrt", 1}}, {TokenType::EXP, {"exp", 1}},
            {TokenType::LOG, {"log", 2}}, {TokenType::MIN, {"min", 2}},
            {TokenType::MAX, {"max", 2}}, {TokenType::RANDOM, {"rand", 0}}
        };

        auto it = builtins.find(type);
        if (it != builtins.end()) {
            std::string name = it->second.first;
            int arity = it->second.second;
            advance();
            return parseBuiltinFunction(name, arity);
        }

        // Default fallback to full expression
        left = parseExpression();

        if (!left) {
            errors.emplace_back("Se esperaba una expresión válida", currentToken->location, "expresión");
            return nullptr;
        }
    }

    // ---- Binary operators
    while (true) {
        TokenType opType = currentToken->type;
        int prec = getPrecedence(opType);

        if (prec < minPrec) break;

        std::string opLexeme = currentToken->lexeme;
        advance();

        auto right = parseExpression();
        if (!right) {
            errors.emplace_back("Se esperaba una expresión después del operador '" + opLexeme + "'", currentToken->location, "expresión");
            return nullptr;
        }

        left = std::make_shared<BinaryOpNode>(opLexeme, left, right, currentToken->location.line);
    }

    return left;
}

std::shared_ptr<ASTNode> Parser::parseBuiltinFunction(const std::string& name, int expectedArgs) {
    expect(TokenType::LPAREN, "(");
    std::vector<std::shared_ptr<ASTNode>> args;

    if (expectedArgs == 0) {
        if (currentToken->type != TokenType::RPAREN) {
            errors.emplace_back("La función '" + name + "' no acepta argumentos", currentToken->location, ")");
        }
    } else {
        args.push_back(parseExpression());

        for (int i = 1; i < expectedArgs; ++i) {
            if (match(TokenType::COMMA)) {
                args.push_back(parseExpression());
            } else {
                errors.emplace_back("Se esperaba ',' entre argumentos en función '" + name + "'", currentToken->location, ",");
                break;
            }
        }

        // Catch extra arguments
        if (match(TokenType::COMMA)) {
            errors.emplace_back("Demasiados argumentos en función '" + name + "'", currentToken->location, ")");
        }
    }

    expect(TokenType::RPAREN, ")");

    return std::make_shared<BuiltInFunctionNode>(name, args, currentToken->location.line);
}

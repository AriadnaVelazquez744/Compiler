#include "Parser.hpp"
#include <iostream>
#include <stdexcept>

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

        // Lookahead: assignment := 
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


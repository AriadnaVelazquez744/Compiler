#pragma once
#include <memory>
#include <vector>
#include "../lexer/Lexer.hpp"
#include "../lexer/Token.hpp"
#include "ParserError.hpp"
#include "../ast/AST.hpp"

class Parser {
public:
    Parser(std::shared_ptr<Lexer> lexer);

    std::vector<std::shared_ptr<ASTNode>> parseProgram();
    const std::vector<ParserError>& getErrors() const;

private:
    std::shared_ptr<Lexer> lexer;
    std::shared_ptr<Token> currentToken;
    std::vector<ParserError> errors;

    // Token management
    void advance();
    bool match(TokenType type);
    void expect(TokenType type, const std::string& expectedMessage);

    // Grammar rule functions
    std::shared_ptr<ASTNode> parseStatement();
    std::shared_ptr<ASTNode> parseExpression();
    std::shared_ptr<ASTNode> parseElemExpr(std::shared_ptr<ASTNode> left = nullptr, int minPrec = 0);
    std::shared_ptr<ASTNode> parseIfExpr();
    std::shared_ptr<ASTNode> parseWhileExpr();
    std::shared_ptr<ASTNode> parseForExpr();
    std::shared_ptr<ASTNode> parseLetExpr();
    std::shared_ptr<ASTNode> parseFuncDecl(bool isInline);
    std::shared_ptr<ASTNode> parseBlockExpr();
    std::vector<std::shared_ptr<ASTNode>> parseBlockBody();
    std::shared_ptr<ASTNode> parseFuncCallExpr();
    std::vector<std::shared_ptr<ASTNode>> parseArgs();
    std::shared_ptr<ASTNode> parseAssignExpr();
    std::vector<Parameter> parseParams();
    std::shared_ptr<ASTNode> parseBody();
    std::vector<LetDeclaration> parseLetDecl();
    std::vector<IfBranch> parseIfBranches();
};

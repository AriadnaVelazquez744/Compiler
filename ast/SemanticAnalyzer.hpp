#pragma once
#include "ASTVisitor.hpp"
#include "SymbolTable.hpp"
#include "SemanticError.hpp"
#include <vector>

class SemanticAnalyzer : public ASTVisitor {
private:
    SymbolTable symbolTable;
    std::vector<SemanticError> errors;

    // Helper para verificar funciones matemáticas
    void checkMathFunction(const std::string& funcName, const std::vector<ASTNode*>& args, int line);

    bool isValidIdentifier(const std::string& name);

public:
    void analyze(ASTNode* root);

    // Métodos visit
    void visit(FunctionDeclarationNode& node) override;
    void visit(BinaryOpNode& node) override;
    void visit(FunctionCallNode& node) override;
    void visit(ASTNode& node) override;
    void visit(LiteralNode& node) override;
    void visit(BlockNode& node) override;
    void visit(VariableDeclarationNode& node) override;
    void visit(IdentifierNode& node) override;
    void visit(LetNode& node) override;
    void visit(AssignmentNode& node) override;
    void visit(IfNode& node) override;
    void visit(WhileNode& node) override;
    void visit(ForNode& node) override;
    void visit(TypeDeclarationNode& node) override;
    void visit(NewNode& node) override;

    const std::vector<SemanticError>& getErrors() const { return errors; }
};
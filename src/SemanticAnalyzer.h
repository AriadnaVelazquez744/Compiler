#pragma once
#include "ASTVisitor.h"
#include "SymbolTable.h"
#include "SemanticError.h"
#include <vector>

class SemanticAnalyzer : public ASTVisitor {
private:
    SymbolTable symbolTable;
    std::vector<SemanticError> errors;

    // Helper para verificar funciones matemáticas
    void checkMathFunction(const std::string& funcName, const std::vector<ASTNode*>& args, int line);

public:
    // Métodos del ASTVisitor
    void visit(ASTNode& node) override;
    void visit(BinaryOpNode& node) override;
    void visit(FunctionCallNode& node) override;
    void visit(LiteralNode& node) override;
    void visit(BlockNode& node) override;
    void visit(VariableDeclarationNode& node) override;
    void visit(IdentifierNode& node) override;

    const std::vector<SemanticError>& getErrors() const { return errors; }
};
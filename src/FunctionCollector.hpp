#pragma once
#include "ASTVisitor.hpp"
#include "SymbolTable.hpp"
#include "SemanticError.hpp"

class FunctionCollector : public ASTVisitor {
    private:
        SymbolTable& symbolTable;
        std::vector<SemanticError>& errors;
    
    public:
        FunctionCollector(SymbolTable& symTable, std::vector<SemanticError>& err);
        virtual ~FunctionCollector() = default;
    
        // Implementar TODOS los métodos de ASTVisitor
        void visit(ASTNode& node) override {} // <--- Añadir esta línea
        void visit(FunctionDeclarationNode& node) override;
        void visit(BinaryOpNode& node) override {}
        void visit(FunctionCallNode& node) override {}
        void visit(LiteralNode& node) override {}
        void visit(BlockNode& node) override {}
        void visit(VariableDeclarationNode& node) override {}
        void visit(IdentifierNode& node) override {}
        void visit(LetNode& node) override {}
        void visit(AssignmentNode& node) override {}
    };
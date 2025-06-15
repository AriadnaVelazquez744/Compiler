//Functioncollector.hpp
#pragma once
#include "../ast/ASTVisitor.hpp"
#include "SymbolTable.hpp"
#include "SemanticError.hpp"

class FunctionCollector : public ASTVisitor {
    private:
        SymbolTable& symbolTable;
        std::vector<SemanticError>& errors;
    
    public:
        FunctionCollector(SymbolTable& symTable, std::vector<SemanticError>& err);
        virtual ~FunctionCollector() = default;

        void addBuiltins();
    
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
        void visit(IfNode& node) override {}
        void visit(WhileNode& node) override {}
        void visit(ForNode& node) override {}
        void visit(TypeDeclarationNode& node) override {}
        void visit(NewInstanceNode& node) override {}
        void visit(UnaryOpNode& node) override {}
        void visit(BuiltInFunctionNode& node) override {}
        void visit(AttributeDeclaration& node) override {}
        void visit(MethodDeclaration& node) override {}
        void visit(MethodCallNode& node) override {}
        void visit(BaseCallNode& node) override {}
        void visit(SelfCallNode& node) override {}
    };
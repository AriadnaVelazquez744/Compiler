#pragma once

// Declaraciones adelantadas de todos los nodos del AST
class ASTNode;
class BinaryOpNode;
class FunctionCallNode;
class LiteralNode;
class BlockNode;
class VariableDeclarationNode;
class IdentifierNode;

class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;

    // Métodos para visitar cada tipo de nodo
    virtual void visit(ASTNode& node) = 0;
    virtual void visit(BinaryOpNode& node) = 0;
    virtual void visit(FunctionCallNode& node) = 0;
    virtual void visit(LiteralNode& node) = 0;
    virtual void visit(BlockNode& node) = 0;
    virtual void visit(VariableDeclarationNode& node) = 0;
    virtual void visit(IdentifierNode& node) = 0;
};
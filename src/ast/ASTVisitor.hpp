#pragma once

// Declaraciones adelantadas de todos los nodos del AST
class ASTNode;
class BinaryOpNode;
class FunctionCallNode;
class LiteralNode;
class BlockNode;
class VariableDeclarationNode;
class IdentifierNode;
class FunctionDeclarationNode; 
class LetNode;
class AssignmentNode;
class IfNode;
class WhileNode;
class ForNode;
class TypeDeclarationNode;
class NewInstanceNode;
class UnaryOpNode;
class BuiltInFunctionNode;
class AttributeDeclaration;
class MethodDeclaration;
class MethodCallNode;
class BaseCallNode;
class SelfCallNode;


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
    virtual void visit(FunctionDeclarationNode& node) = 0;
    virtual void visit(LetNode& node) = 0;
    virtual void visit(AssignmentNode& node) = 0;
    virtual void visit(IfNode& node) = 0;
    virtual void visit(WhileNode& node) = 0;
    virtual void visit(ForNode& node) = 0;
    virtual void visit(TypeDeclarationNode& node) = 0;
    virtual void visit(NewInstanceNode& node) = 0;
    virtual void visit(AttributeDeclaration& node) = 0;
    virtual void visit(MethodDeclaration& node) = 0;
    virtual void visit(MethodCallNode& node) = 0;
    virtual void visit(BaseCallNode& node) = 0;
    virtual void visit(SelfCallNode& node) = 0;
    virtual void visit(UnaryOpNode& node) = 0;
    virtual void visit(BuiltInFunctionNode& node) = 0;
};
#pragma once

#include "../ast/ASTVisitor.hpp"
#include "llvm/IR/Value.h"

class CodeGenContext;

// This class implements the visitor pattern for generating LLVM IR
class LLVMGenerator : public ASTVisitor {
private:
    CodeGenContext& context;

public:
    LLVMGenerator(CodeGenContext& ctx);

    // Implemented nodes
    void visit(class LiteralNode& node) override;
    void visit(class BinaryOpNode& node) override;
    void visit(class UnaryOpNode& node) override;
    void visit(class BuiltInFunctionNode& node) override;
    void visit(class BlockNode& node) override;
    void visit(class IdentifierNode& node) override;
    void visit(class FunctionDeclarationNode& node) override;
    void visit(class FunctionCallNode& node) override;
    void visit(class LetNode& node) override;
    void visit(class AssignmentNode& node) override;
    void visit(class IfNode& node) override;
    void visit(class WhileNode& node) override;
    void visit(class ForNode& node) override;
    void visit(class TypeDeclarationNode& node) override;
    void visit(class NewInstanceNode& node) override;
    void visit(class MethodCallNode& node) override;
    void visit(class BaseCallNode& node) override;
    void visit(class SelfCallNode& node) override;
    void visit(TypeTestNode& node) override {}
    void visit(TypeCastNode& node) override {}
    void visit(AttributeCallNode& node) override {}
    
    // Stubs for other nodes
    void visit(ASTNode&) override {}
    void visit(VariableDeclarationNode&) override {}
    void visit(AttributeDeclaration& node) override {}
    void visit(MethodDeclaration& node) override {}
};

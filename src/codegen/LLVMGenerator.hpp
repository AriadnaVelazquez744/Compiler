#pragma once

#include "../ast/AST.hpp"
#include "CodeGenContext.hpp"
#include "TypeSystem.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>
#include <vector>

class CodeGenContext;

// This class implements the visitor pattern for generating LLVM IR
class LLVMGenerator : public ASTVisitor {
private:
    CodeGenContext& context;

public:
    LLVMGenerator(CodeGenContext& ctx);

    // Access methods for TypeSystem integration
    llvm::IRBuilder<>& getBuilder();
    void setBuilder(llvm::IRBuilder<>* newBuilder);
    std::vector<llvm::Value*>& getValueStack();

    // AST Visitor methods
    void visit(ASTNode&) override {}
    void visit(class BinaryOpNode& node) override;
    void visit(class UnaryOpNode& node) override;
    void visit(class LiteralNode& node) override;
    void visit(class IdentifierNode& node) override;
    void visit(VariableDeclarationNode&) override {}
    void visit(class AssignmentNode& node) override;
    void visit(class BlockNode& node) override;
    void visit(class FunctionDeclarationNode& node) override;
    void visit(class FunctionCallNode& node) override;
    void visit(class BuiltInFunctionNode& node) override;
    void visit(class LetNode& node) override;
    void visit(class IfNode& node) override;
    void visit(class WhileNode& node) override;
    void visit(class ForNode& node) override;
    void visit(class TypeDeclarationNode& node) override;
    void visit(class NewInstanceNode& node) override;
    void visit(class MethodCallNode& node) override;
    void visit(class SelfCallNode& node) override;
    void visit(class BaseCallNode& node) override;
    void visit(AttributeDeclaration&) override {}
    void visit(MethodDeclaration&) override {}
};

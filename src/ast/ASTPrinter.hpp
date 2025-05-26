#pragma once
#include "ASTVisitor.hpp"
#include <iostream>
#include <memory>

class ASTPrinter : public ASTVisitor {
    int indent = 0;

    void printIndent() const {
        for (int i = 0; i < indent; ++i)
            std::cout << "  ";
    }

public:
    void visit(BinaryOpNode& node) override {
        printIndent();
        std::cout << "BinaryOp(" << node.op << ")\n";

        indent++;
        node.left->accept(*this);
        node.right->accept(*this);
        indent--;
    }

    void visit(UnaryOpNode& node) override {
        printIndent();
        std::cout << "UnaryOp(" << node.op << ")\n";

        indent++;
        node.operand->accept(*this);
        indent--;
    }

    void visit(LiteralNode& node) override {
        printIndent();
        std::cout << "Literal(" << node.value << ")\n";
    }

    void visit(IdentifierNode& node) override {
        printIndent();
        std::cout << "Identifier(" << node.name << ")\n";
    }

    void visit(BlockNode& node) override {
        printIndent();
        std::cout << "Block\n";
        indent++;
        for (auto& expr : node.expressions) {
            expr->accept(*this);
        }
        indent--;
    }

    void visit(FunctionCallNode& node) override {
        printIndent();
        std::cout << "Call(" << node.funcName << ")\n";
        indent++;
        for (auto& arg : node.args)
            arg->accept(*this);
        indent--;
    }

    void visit(BuiltInFunctionNode& node) override {
        printIndent();
        std::cout << "BuiltIn(" << node.name << ")\n";
        indent++;
        for (auto& arg : node.args)
            arg->accept(*this);
        indent--;
    }

    // Add stubs for all missing node types:
    void visit(ASTNode&) override {}
    void visit(VariableDeclarationNode&) override {}
    void visit(FunctionDeclarationNode&) override {}
    void visit(LetNode&) override {}
    void visit(AssignmentNode&) override {}
    void visit(IfNode&) override {}
    void visit(WhileNode&) override {}
    void visit(ForNode&) override {}
    void visit(TypeDeclarationNode&) override {}
    void visit(NewInstanceNode&) override {}
    void visit(MethodCallNode&) override {}    
};

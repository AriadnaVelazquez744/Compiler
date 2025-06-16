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

    void visit(ASTNode&) override {}
    
    void visit(VariableDeclarationNode& node) override {
        printIndent();
        std::cout << "VariableDeclaration(" << node.name << ")\n";
        indent++;
        if (node.initializer) {
            node.initializer->accept(*this);
        }
        indent--;
    }

    void visit(FunctionDeclarationNode& node) override {
        printIndent();
        std::cout << "FunctionDeclaration(" << node.name << ")\n";
        indent++;
        if (node.body) {
            node.body->accept(*this);
        }
        indent--;
    }

    void visit(LetNode& node) override {
        printIndent();
        std::cout << "Let\n";
        indent++;
        if (node.declarations) {
            for (const auto& decl : *node.declarations) {
                printIndent();
                std::cout << "Declaration(" << decl.name << ")\n";
                if (decl.initializer) {
                    decl.initializer->accept(*this);
                }
            }
        }
        if (node.body) {
            node.body->accept(*this);
        }
        indent--;
    }

    void visit(AssignmentNode& node) override {
        printIndent();
        std::cout << "Assignment\n";
        indent++;
        if (node.name) {
            node.name->accept(*this);
        }
        if (node.rhs) {
            node.rhs->accept(*this);
        }
        indent--;
    }

    void visit(IfNode& node) override {
        printIndent();
        std::cout << "If\n";
        indent++;
        if (node.branches) {
            for (const auto& branch : *node.branches) {
                printIndent();
                std::cout << "Branch\n";
                indent++;
                if (branch.condition) {
                    branch.condition->accept(*this);
                }
                if (branch.body) {
                    branch.body->accept(*this);
                }
                indent--;
            }
        }
        if (node.elseBody) {
            printIndent();
            std::cout << "Else\n";
            indent++;
            node.elseBody->accept(*this);
            indent--;
        }
        indent--;
    }

    void visit(WhileNode& node) override {
        printIndent();
        std::cout << "While\n";
        indent++;
        if (node.condition) {
            node.condition->accept(*this);
        }
        if (node.body) {
            node.body->accept(*this);
        }
        indent--;
    }

    void visit(ForNode& node) override {
        printIndent();
        std::cout << "For(" << node.varName << ")\n";
        indent++;
        if (node.init_range) {
            node.init_range->accept(*this);
        }
        if (node.end_range) {
            node.end_range->accept(*this);
        }
        if (node.body) {
            node.body->accept(*this);
        }
        indent--;
    }

    void visit(TypeDeclarationNode& node) override {
        printIndent();
        std::cout << "TypeDeclaration(" << node.name << ")\n";
        indent++;
        if (node.body) {
            if (node.body->attributes) {
                for (const auto& attr : *node.body->attributes) {
                    printIndent();
                    std::cout << "Attribute(" << attr.name << ")\n";
                    if (attr.initializer) {
                        attr.initializer->accept(*this);
                    }
                }
            }
            if (node.body->methods) {
                for (const auto& method : *node.body->methods) {
                    printIndent();
                    std::cout << "Method(" << method.name << ")\n";
                    if (method.body) {
                        method.body->accept(*this);
                    }
                }
            }
        }
        indent--;
    }

    void visit(NewInstanceNode& node) override {
        printIndent();
        std::cout << "NewInstance(" << node.typeName << ")\n";
        indent++;
        for (auto& arg : node.args) {
            arg->accept(*this);
        }
        indent--;
    }

    void visit(MethodCallNode& node) override {
        printIndent();
        std::cout << "MethodCall(" << node.methodName << ")\n";
        indent++;
        if (node.object) {
            node.object->accept(*this);
        }
        for (auto& arg : node.args) {
            arg->accept(*this);
        }
        indent--;
    }

    void visit(AttributeDeclaration& node) override {
        printIndent();
        std::cout << "AttributeDeclaration(" << node.name << ")\n";
        indent++;
        if (node.initializer) {
            node.initializer->accept(*this);
        }
        indent--;
    }

    void visit(MethodDeclaration& node) override {
        printIndent();
        std::cout << "MethodDeclaration(" << node.name << ")\n";
        indent++;
        if (node.body) {
            node.body->accept(*this);
        }
        indent--;
    }

    void visit(BaseCallNode& node) override {
        printIndent();
        std::cout << "BaseCall\n";
        indent++;
        for (auto& arg : node.args) {
            arg->accept(*this);
        }
        indent--;
    }

    void visit(SelfCallNode& node) override {
        printIndent();
        std::cout << "SelfCall(" << node.varName << ")\n";
    }
};

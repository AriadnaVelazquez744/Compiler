#include "SemanticAnalyzer.h"
#include "AST.h"

void SemanticAnalyzer::visit(ASTNode& node) {}

// Implementación de visitas a nodos
void SemanticAnalyzer::visit(BinaryOpNode& node) {
    node.left->accept(*this);
    node.right->accept(*this);

    std::string leftType = node.left->type();
    std::string rightType = node.right->type();

    if (node.op == "@") {
        if (leftType != "string" && leftType != "number") {
            errors.emplace_back("Operando izquierdo de @ debe ser string o number", node.line());
        }
        if (rightType != "string" && rightType != "number") {
            errors.emplace_back("Operando derecho de @ debe ser string o number", node.line());
        }
        node._type = "string";
    } else {
        if (leftType != "number" || rightType != "number") {
            errors.emplace_back("Operandos de " + node.op + " deben ser números", node.line());
        }
        node._type = "number";
    }
}

// ------------------------------------------------------

void SemanticAnalyzer::visit(FunctionCallNode& node) {
    if (node.funcName == "print") {
        for (auto arg : node.args) {
            arg->accept(*this);
        }
        node._type = "void";
    } else if (node.funcName == "sin" || node.funcName == "cos" || node.funcName == "exp") {
        if (node.args.size() != 1) {
            errors.emplace_back("Función " + node.funcName + " requiere 1 argumento", node.line());
        } else {
            node.args[0]->accept(*this);
            if (node.args[0]->type() != "number") {
                errors.emplace_back("Argumento de " + node.funcName + " debe ser número", node.line());
            }
        }
        node._type = "number";
    } else if (node.funcName == "log") {
        if (node.args.size() != 2) {
            errors.emplace_back("Función log requiere 2 argumentos", node.line());
        } else {
            node.args[0]->accept(*this);
            node.args[1]->accept(*this);
            if (node.args[0]->type() != "number" || node.args[1]->type() != "number") {
                errors.emplace_back("Argumentos de log deben ser números", node.line());
            }
        }
        node._type = "number";
    } else if (node.funcName == "rand") {
        if (!node.args.empty()) {
            errors.emplace_back("Función rand no requiere argumentos", node.line());
        }
        node._type = "number";
    } else {
        errors.emplace_back("Función '" + node.funcName + "' no definida", node.line());
    }
}

// ------------------------------------------------------

void SemanticAnalyzer::visit(LiteralNode& node) {
    // El tipo ya está definido (number, string, boolean)
    node._type = node._type;
}

// ------------------------------------------------------

void SemanticAnalyzer::visit(BlockNode& node) {
    symbolTable.enterScope();
    for (auto expr : node.expressions) {
        expr->accept(*this);
    }
    symbolTable.exitScope();
    node._type = "void";
}

// ------------------------------------------------------

void SemanticAnalyzer::visit(VariableDeclarationNode& node) {
    if (symbolTable.existsInCurrentScope(node.name)) {
        errors.emplace_back("Variable '" + node.name + "' ya declarada", node.line());
        return;
    }

    if (node.initializer) {
        node.initializer->accept(*this);
        std::string initType = node.initializer->type();
        if (!node.declaredType.empty() && node.declaredType != initType) {
            errors.emplace_back("Tipo declarado no coincide con el inicializador", node.line());
        }
        node._type = !node.declaredType.empty() ? node.declaredType : initType;
    } else {
        node._type = node.declaredType;
    }

    symbolTable.addSymbol(node.name, node._type, !node.isMutable);
}

// ------------------------------------------------------

void SemanticAnalyzer::visit(IdentifierNode& node) {
    Symbol* symbol = symbolTable.lookup(node.name);
    if (!symbol) {
        errors.emplace_back("Variable '" + node.name + "' no declarada", node.line());
        return;
    }
    node._type = symbol->type;
}
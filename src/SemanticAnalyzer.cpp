#include "SemanticAnalyzer.hpp"
#include "AST.hpp"
#include "FunctionCollector.hpp"
#include <cctype>


void SemanticAnalyzer::analyze(ASTNode* root) {
    // Fase 1: Recolectar funciones
    FunctionCollector collector(symbolTable, errors);
    root->accept(collector);

    // Fase 2: Análisis semántico completo
    root->accept(*this);
}

void SemanticAnalyzer::visit(ASTNode& node) {}

// Implementación de visitas a nodos

void SemanticAnalyzer::visit(FunctionDeclarationNode& node) {
    symbolTable.enterScope();

    // Verificar parámetros únicos
    std::unordered_map<std::string, bool> paramsSeen;
    for (const auto& param : node.params) {
        if (paramsSeen.count(param.name)) {
            errors.emplace_back("Parámetro duplicado '" + param.name + "'", node.line());
        } else {
            paramsSeen[param.name] = true;
            symbolTable.addSymbol(param.name, param.type, false);
        }
    }

    // Analizar cuerpo
    node.body->accept(*this);
    std::string bodyType = node.body->type();

    // Verificar tipo de retorno
    if (!node.returnType.empty() && node.returnType != bodyType) {
        errors.emplace_back("Tipo de retorno incorrecto en función '" + node.name + "'", node.line());
    }

    symbolTable.exitScope();
}

void SemanticAnalyzer::visit(FunctionCallNode& node) {
    // Manejar funciones built-in primero
    if (node.funcName == "print") {
        for (auto arg : node.args) {
            arg->accept(*this);
        }
        node._type = "void";
    } 
    else if (node.funcName == "sin" || node.funcName == "cos" || node.funcName == "exp") {
        if (node.args.size() != 1) {
            errors.emplace_back("Función " + node.funcName + " requiere 1 argumento", node.line());
        } else {
            node.args[0]->accept(*this);
            if (node.args[0]->type() != "number") {
                errors.emplace_back("Argumento de " + node.funcName + " debe ser número", node.line());
            }
        }
        node._type = "number";
    } 
    else if (node.funcName == "log") {
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
    } 
    else if (node.funcName == "rand") {
        if (!node.args.empty()) {
            errors.emplace_back("Función rand no requiere argumentos", node.line());
        }
        node._type = "number";
    } 
    // Verificar funciones definidas por el usuario
    else {
        Symbol* funcSymbol = symbolTable.lookup(node.funcName);
        if (!funcSymbol || funcSymbol->kind != "function") {
            errors.emplace_back("Función '" + node.funcName + "' no definida", node.line());
            return;
        }

        // Verificar número de argumentos
        if (node.args.size() != funcSymbol->params.size()) {
            errors.emplace_back("Número incorrecto de argumentos para '" + node.funcName + "'", node.line());
            return;
        }

        // Verificar tipos de argumentos
        for (size_t i = 0; i < node.args.size(); ++i) {
            node.args[i]->accept(*this);
            std::string argType = node.args[i]->type();
            if (argType != funcSymbol->params[i]) {
                errors.emplace_back("Tipo incorrecto para argumento " + std::to_string(i+1) + " en '" + node.funcName + "'", node.line());
            }
        }

        node._type = funcSymbol->type;
    }
}

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

bool SemanticAnalyzer::isValidIdentifier(const std::string& name) {
    if (name.empty()) return false;
    if (!std::isalpha(name[0])) return false; // Debe empezar con letra
    for (char c : name) {
        if (!std::isalnum(c) && c != '_') return false; // Solo alfanumérico y _
    }
    return true;
}

void SemanticAnalyzer::visit(LetNode& node) {
    symbolTable.enterScope(); // Nuevo ámbito

    for (auto& decl : node.declarations) {
        // Verificar nombre válido
        if (!isValidIdentifier(decl.name)) {
            errors.emplace_back("Nombre inválido: '" + decl.name + "'", node.line());
        }

        // Verificar duplicados en el mismo let
        if (symbolTable.existsInCurrentScope(decl.name)) {
            errors.emplace_back("Variable '" + decl.name + "' ya declarada en este ámbito", node.line());
            continue;
        }

        // Analizar inicializador
        decl.initializer->accept(*this);
        std::string initType = decl.initializer->type();

        // Validar tipo declarado vs inferido
        if (!decl.declaredType.empty() && decl.declaredType != initType) {
            errors.emplace_back("Tipo declarado '" + decl.declaredType + "' no coincide con inicializador '" + initType + "'", node.line());
        }

        // Registrar en tabla de símbolos (no constante)
        symbolTable.addSymbol(decl.name, decl.declaredType.empty() ? initType : decl.declaredType, false);
    }

    // Analizar cuerpo
    node.body->accept(*this);
    node._type = node.body->type();

    symbolTable.exitScope();
}

void SemanticAnalyzer::visit(AssignmentNode& node) {
    Symbol* symbol = symbolTable.lookup(node.name);
    
    if (!symbol) {
        errors.emplace_back("Variable '" + node.name + "' no declarada", node.line());
        return;
    }

    if (symbol->is_const) {
        errors.emplace_back("No se puede reasignar la constante '" + node.name + "'", node.line());
    }

    node.rhs->accept(*this);
    std::string rhsType = node.rhs->type();

    if (rhsType != symbol->type) {
        errors.emplace_back("Tipo incorrecto en asignación: esperado '" + symbol->type + "', obtenido '" + rhsType + "'", node.line());
    }

    node._type = symbol->type; // Tipo de la expresión es el de la variable
}
#include "../include/SemanticAnalyzer.hpp"
#include "../include/AST.hpp"
#include "../include/FunctionCollector.hpp"
#include <cctype>
#include <set> 
#include <iostream>


void SemanticAnalyzer::analyze(ASTNode* root) {
    // Fase 1: Recolectar funciones
    FunctionCollector collector(symbolTable, errors);
    collector.addBuiltins();
    root->accept(collector);

    // Fase 2: Análisis semántico completo
    root->accept(*this);
}

void SemanticAnalyzer::visit(ASTNode& node) {}

// Implementación de visitas a nodos

void SemanticAnalyzer::visit(UnaryOpNode& node) {
    node.operand->accept(*this); // Analiza operand
    std::string operandType = node.operand->type();

    if (node.op == "-") {
        if (operandType != "Number") {
            errors.emplace_back("El operador '-' requiere un operando de tipo Number", node.line());
        }
        node._type = "Number";
    } else if (node.op == "!") {
        if (operandType != "Boolean") {
            errors.emplace_back("El operador '!' requiere un operando de tipo Boolean", node.line());
        }
        node._type = "Boolean";
    } else {
        errors.emplace_back("Operador unario desconocido: " + node.op, node.line());
        node._type = "Unknown";
    }
}

void SemanticAnalyzer::visit(BuiltInFunctionNode& node) {
    for (ASTNode* arg : node.args) {
        arg->accept(*this);
    }

    const std::string& fn = node.name;
    size_t arity = node.args.size();

    if ((fn == "sin" || fn == "cos" || fn == "exp" || fn == "sqrt") && arity == 1) {
        if (node.args[0]->type() != "Number") {
            std::cerr << "Error: el argumento de '" << fn << "' debe ser numérico.\n";
            node._type = "Error";
            return;
        }
        node._type = "Number";
    } else if ((fn == "min" || fn == "max") && arity == 2) {
        if (node.args[0]->type() == "Number" && node.args[1]->type() == "Number") {
            node._type = "Number";
        } else {
            std::cerr << "Error: los argumentos de '" << fn << "' deben ser numéricos.\n";
            node._type = "Error";
        }
    } else if (fn == "log" && arity == 2) {
        if (node.args[0]->type() == "Number" && node.args[1]->type() == "Number") {
            node._type = "Number";
        } else {
            std::cerr << "Error: los argumentos de 'log' deben ser numéricos.\n";
            node._type = "Error";
        }
    } else if (fn == "rand" && arity == 0) {
        node._type = "Number";
    } else {
        std::cerr << "Error: función '" << fn << "' con " << arity << " argumentos no es válida.\n";
        node._type = "Error";
    }
}



void SemanticAnalyzer::visit(FunctionDeclarationNode& node) {
    symbolTable.enterScope();

    // Verificar parámetros únicos
    std::unordered_map<std::string, bool> paramsSeen;
    for (const auto& param : *node.params) {
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

    if (node.funcName == "base") {
        // Verificar que estamos en un método de una clase derivada
        Symbol* selfSym = symbolTable.lookup("self");
        if (!selfSym) {
            errors.emplace_back("'base' solo puede usarse en métodos", node.line());
            return;
        }
        TypeSymbol* typeSym = symbolTable.lookupType(selfSym->type);
        if (!typeSym || typeSym->parentType == "Object") {
            errors.emplace_back("'base' no disponible en este contexto", node.line());
        }
    }

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

    // Operadores de comparación (devuelven boolean)
    const std::set<std::string> comparisonOps = {"==", "!=", "<", ">", "<=", ">="};
    
    if (comparisonOps.count(node.op)) {
        if (leftType != "number" || rightType != "number") {
            errors.emplace_back("Operandos de " + node.op + " deben ser números", node.line());
            node._type = "error"; // Marcar como error si los tipos son inválidos
        } else {
            node._type = "boolean"; // Solo asignar boolean si los operandos son válidos
        }
    }
    // Operador de concatenación @

    else if (node.op == "@") {
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

    for (auto& decl : *node.declarations) {
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
    
    if (node.name == "self") {
        errors.emplace_back("No se puede reasignar 'self'", node.line());
        return;
    }

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

void SemanticAnalyzer::visit(IfNode& node) {
    // 1. Verificar que existe else
    if (!node.elseBody) {
        errors.emplace_back("La expresión if debe tener cláusula else", node.line());
        return;
    }

    std::vector<std::string> branchTypes;
    bool hasErrors = false;

    // 2. Verificar condiciones y recolectar tipos
    for (auto& branch : *node.branches) {
        // Analizar condición
        branch.condition->accept(*this);
        std::string condType = branch.condition->type();
        if (condType != "boolean") {
            errors.emplace_back("Condición debe ser booleana", branch.condition->line());
            hasErrors = true;
        }

        // Analizar cuerpo de la rama
        branch.body->accept(*this);
        branchTypes.push_back(branch.body->type());
    }

    // 3. Analizar else
    node.elseBody->accept(*this);
    branchTypes.push_back(node.elseBody->type());

    // 4. Verificar compatibilidad de tipos
    if (!hasErrors) {
        std::string commonType = branchTypes[0];
        for (size_t i = 1; i < branchTypes.size(); ++i) {
            if (branchTypes[i] != commonType) {
                errors.emplace_back("Tipos incompatibles en ramas del if", node.line());
                break;
            }
        }
        node._type = commonType;
    }
}

void SemanticAnalyzer::visit(WhileNode& node) {
    // Verificar condición es booleana
    node.condition->accept(*this);
    std::string condType = node.condition->type();
    if (condType != "boolean") {
        errors.emplace_back("Condición del while debe ser booleana", node.line());
    }

    // Analizar cuerpo
    node.body->accept(*this);
    node._type = node.body->type(); // Tipo del while = tipo del cuerpo
}

void SemanticAnalyzer::visit(ForNode& node) {
    node.init_range->accept(*this);
    node.end_range->accept(*this);

    if (node.init_range->type() != "Number" || node.end_range->type() != "Number") {
        errors.emplace_back("Los límites del 'for' deben ser de tipo Number", node.line());
        return;
    }

    symbolTable.enterScope();
    symbolTable.addSymbol(node.varName, "Number", false);

    node.body->accept(*this);
    node._type = node.body->type();

    symbolTable.exitScope();
}


void SemanticAnalyzer::visit(TypeDeclarationNode& node) {
    // 1. Verificar que el tipo no existe
    if (symbolTable.lookupType(node.name)) {
        errors.emplace_back("Tipo '" + node.name + "' ya declarado", node.line());
        return;
    }

    // 2. Verificar herencia de tipos básicos
    const std::set<std::string> builtinTypes = {"Number", "String", "Boolean"};
    if (builtinTypes.count(node.parentType)) {
        errors.emplace_back("No se puede heredar de tipos básicos", node.line());
        return;
    }

    // 3. Registrar tipo en la tabla de símbolos
    if (!symbolTable.addType(node.name, node.parentType.empty() ? "Object" : node.parentType, node.typeParams)) {
        errors.emplace_back("Tipo '" + node.name + "' ya declarado", node.line());
        return;
    }

    TypeSymbol* typeSym = symbolTable.lookupType(node.name);
    if (!typeSym) return;

    // 4. Analizar atributos (sin acceso a self)
    for (size_t i = 0; i < node.attributeInits.size(); ++i) {
        node.attributeInits[i]->accept(*this);
        // Registrar atributo como Symbol
        symbolTable.addTypeAttribute(
            node.name, 
            "attr" + std::to_string(i), 
            node.attributeInits[i]->type()
        );
    }

    // 5. Analizar métodos (con acceso a self)
    for (auto method : node.methods) {
        symbolTable.enterScope();
        symbolTable.addSymbol("self", node.name, true); // self es constante

        // Registrar parámetros del método
        for (auto& param : *method->params) {
            symbolTable.addSymbol(param.name, param.type, false);
        }

        method->body->accept(*this);
        symbolTable.exitScope();

        // Registrar método en el tipo
        std::vector<std::string> paramTypes;
        for (auto& param : *method->params) {
            paramTypes.push_back(param.type);
        }
        symbolTable.addTypeMethod(
            node.name,
            method->name,
            method->returnType,
            paramTypes
        );

        // Verificar firma con el padre
        TypeSymbol* parent = symbolTable.lookupType(typeSym->parentType);
        if (parent) {
            auto it = parent->methods.find(method->name);
            if (it != parent->methods.end()) {
                Symbol& parentMethod = it->second;
                if (parentMethod.params != paramTypes || parentMethod.type != method->returnType) {
                    errors.emplace_back("Firma de método no coincide con el padre", method->line());
                }
            }
        }
    }
}

void SemanticAnalyzer::visit(NewNode& node) {
    TypeSymbol* typeSym = symbolTable.lookupType(node.typeName);
    if (!typeSym) {
        errors.emplace_back("Tipo '" + node.typeName + "' no definido", node.line());
        return;
    }

    // Verificar argumentos del constructor
    if (node.args.size() != typeSym->typeParams.size()) {
        errors.emplace_back("Número incorrecto de argumentos para el tipo", node.line());
        return;
    }

    for (size_t i = 0; i < node.args.size(); ++i) {
        node.args[i]->accept(*this);
        std::string argType = node.args[i]->type();
        if (argType != "Number") { // Ejemplo: validar tipo según parámetro genérico
            errors.emplace_back("Tipo incorrecto para argumento del constructor", node.line());
        }
    }

    node._type = node.typeName;
}
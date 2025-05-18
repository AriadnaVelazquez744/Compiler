#include "SemanticAnalyzer.hpp"
#include "../ast/AST.hpp"
#include "FunctionCollector.hpp"
#include <cctype>
#include <set> 
#include <iostream>


void SemanticAnalyzer::analyze(const std::vector<ASTNode*>& nodes) {
    std::cout << "Entra en analyze." << std::endl;

    FunctionCollector collector(symbolTable, errors);
    collector.addBuiltins();
    std::cout << "Builtins agregados." << std::endl;

    for (ASTNode* node : nodes) {
        if (!node) {
            std::cerr << "Nodo nulo en AST." << std::endl;
            continue;
        }

        std::cout << "Recolectando funciones para nodo tipo: " << typeid(*node).name() << std::endl;
        node->accept(collector);
    }

    std::cout << "Fase de recolección completada." << std::endl;

    for (ASTNode* node : nodes) {
        try {
            std::cout << "Análisis semántico de: " << typeid(*node).name() << std::endl;
            node->accept(*this);
        } catch (const std::exception& e) {
            std::cerr << "Error durante análisis semántico: " << e.what() << std::endl;
        }
    }

    if (!errors.empty()) {
        std::cerr << "Errores semánticos encontrados:\n";
        for (const auto& e : errors) {
            std::cerr << "- Línea " << e.line << ": " << e.message << "\n";
        }
    } else {
        std::cout << "No se encontraron errores semánticos.\n";
    }

    if (!errors.empty()) {
    std::cerr << "Errores semánticos presentes. Abortando ejecución.\n";
    exit(1);
    }


    std::cout << "Análisis semántico completado." << std::endl;
}

void SemanticAnalyzer::visit(ASTNode& node) {}

// Implementación de visitas a nodos

void SemanticAnalyzer::visit(UnaryOpNode& node) {
    node.operand->accept(*this); // Analiza operand
    std::string operandType = node.operand->type();

    if (node.op == "-") {
        if (operandType != "Number") {
            errors.emplace_back("El operador '-' requiere un operando de tipo Number", node.line());
            node._type = "Error";
        }
        node._type = "Number";
    } else if (node.op == "!") {
        if (operandType != "Boolean") {
            errors.emplace_back("El operador '!' requiere un operando de tipo Boolean", node.line());
            node._type = "Error";
        }
        node._type = "Boolean";
    } else {
        errors.emplace_back("Operador unario desconocido: " + node.op, node.line());
        node._type = "Error";;
    }
}

void SemanticAnalyzer::visit(BuiltInFunctionNode& node) {
    for (ASTNode* arg : node.args) {
        arg->accept(*this);
    }

    const std::string& fn = node.name;
    size_t arity = node.args.size();

    for (ASTNode* arg : node.args) {
        arg->accept(*this);
    }

    if (fn == "print") {
        if (arity != 1) {
            errors.emplace_back("La función 'print' requiere exactamente 1 argumento", node.line());
            node._type = "Error";
            return;
        }
        node._type = node.args[0]->type(); // El tipo es el del argumento impreso
    }
    else if (fn == "sin" || fn == "cos" || fn == "exp" || fn == "sqrt") {
        if (node.args.size() != 1) {
            errors.emplace_back("Función " + fn + " requiere 1 argumento", node.line());
            node._type = "Error";
        } else if (node.args[0]->type() != "Number") {
            errors.emplace_back("El argumento de '" + fn + "' debe ser un número", node.line());
            node._type = "Error";
        } else {
            node._type = "Number";
        }

    } 
    else if (fn == "log") {
        if (arity != 2) {
            errors.emplace_back("La función 'log' requiere 2 argumentos", node.line());
            node._type = "Error";
        } else if (node.args[0]->type() != "Number" || node.args[1]->type() != "Number") {
            errors.emplace_back("Los argumentos de 'log' deben ser numéricos", node.line());
            node._type = "Error";
        } else {
            node._type = "Number";
        }
    }
    else if (fn == "rand") {
        if (arity != 0) {
            errors.emplace_back("La función 'rand' no acepta argumentos", node.line());
            node._type = "Error";
        } else {
            node._type = "Number";
        }
    }
    else if (fn == "min" || fn == "max") {
        if (arity != 2) {
            errors.emplace_back("La función '" + fn + "' requiere 2 argumentos", node.line());
            node._type = "Error";
        } else if (node.args[0]->type() != "Number" || node.args[1]->type() != "Number") {
            errors.emplace_back("Los argumentos de '" + fn + "' deben ser numéricos", node.line());
            node._type = "Error";
        } else {
            node._type = "Number";
        }
    }
    else {
        errors.emplace_back("Función builtin '" + fn + "' no reconocida", node.line());
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
            node._type = "Error";
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
        node._type = "Error";
    }

    symbolTable.exitScope();
}

void SemanticAnalyzer::visit(FunctionCallNode& node) {

    if (node.funcName == "base") {
        // Verificar que estamos en un método de una clase derivada
        Symbol* selfSym = symbolTable.lookup("self");
        if (!selfSym) {
            errors.emplace_back("'base' solo puede usarse en métodos", node.line());
            node._type = "Error";
            return;
        }
        TypeSymbol* typeSym = symbolTable.lookupType(selfSym->type);
        if (!typeSym || typeSym->parentType == "Object") {
            errors.emplace_back("'base' no disponible en este contexto", node.line());
            node._type = "Error";
        }
    }

    // Manejar funciones built-in primero
    if (node.funcName == "print") {
        for (auto arg : node.args) {
            arg->accept(*this);
        }
        node._type = "void";
    } 
    // else if (node.funcName == "sin" || node.funcName == "cos" || node.funcName == "exp") {
    //     if (node.args.size() != 1) {
    //         errors.emplace_back("Función " + node.funcName + " requiere 1 argumento", node.line());
    //     } else {
    //         node.args[0]->accept(*this);
    //         if (node.args[0]->type() != "number") {
    //             errors.emplace_back("Argumento de " + node.funcName + " debe ser número", node.line());
    //         }
    //     }
    //     node._type = "number";
    // } 
    // else if (node.funcName == "log") {
    //     if (node.args.size() != 2) {
    //         errors.emplace_back("Función log requiere 2 argumentos", node.line());
    //     } else {
    //         node.args[0]->accept(*this);
    //         node.args[1]->accept(*this);
    //         if (node.args[0]->type() != "number" || node.args[1]->type() != "number") {
    //             errors.emplace_back("Argumentos de log deben ser números", node.line());
    //         }
    //     }
    //     node._type = "number";
    // } 
    // else if (node.funcName == "rand") {
    //     if (!node.args.empty()) {
    //         errors.emplace_back("Función rand no requiere argumentos", node.line());
    //     }
    //     node._type = "number";
    // } 
    // Verificar funciones definidas por el usuario
    else {
        Symbol* funcSymbol = symbolTable.lookup(node.funcName);
        if (!funcSymbol || funcSymbol->kind != "function") {
            errors.emplace_back("Función '" + node.funcName + "' no definida", node.line());
            node._type = "Error";
            return;
        }

        // Verificar número de argumentos
        if (node.args.size() != funcSymbol->params.size()) {
            errors.emplace_back("Número incorrecto de argumentos para '" + node.funcName + "'", node.line());
            node._type = "Error";
            return;
        }

        // Verificar tipos de argumentos
        for (size_t i = 0; i < node.args.size(); ++i) {
            node.args[i]->accept(*this);
            std::string argType = node.args[i]->type();
            if (argType != funcSymbol->params[i]) {
                errors.emplace_back("Tipo incorrecto para argumento " + std::to_string(i+1) + " en '" + node.funcName + "'", node.line());
                node._type = "Error";
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
        if (node.op == "==" || node.op == "!=") {
            if (leftType != rightType)
            {
                errors.emplace_back("Operandos de " + node.op + " deben ser iguales", node.line());
                node._type = "Error"; // Marcar como error si los tipos son inválidos
            }
            else {
            node._type = "Boolean"; // Solo asignar boolean si los operandos son válidos
            }
            } 
        else if (leftType != "Number" || rightType != "Number") {
            errors.emplace_back("Operandos de " + node.op + " deben ser números", node.line());
            node._type = "error"; // Marcar como error si los tipos son inválidos
        } else {
            node._type = "Boolean"; // Solo asignar boolean si los operandos son válidos
        }
    }
    else if (node.op == "&" || node.op == "|")
    {
       if (leftType != "Boolean" || rightType != "Boolean") {
            errors.emplace_back("Operandos de " + node.op + " deben ser booleanos", node.line());
            node._type = "Error"; // Marcar como error si los tipos son inválidos
        } else {
            node._type = "Boolean"; // Solo asignar boolean si los operandos son válidos
        }
    }    // Operador de concatenación @

    else if (node.op == "@" || node.op == "@@") {
        if (leftType != "String" && leftType != "Number") {
            errors.emplace_back("Operando izquierdo de @ debe ser string o number", node.line());
            node._type = "Error";
        }
        if (rightType != "String" && rightType != "Number") {
            errors.emplace_back("Operando derecho de @ debe ser string o number", node.line());
            node._type = "Error";
        }
        node._type = "String";
    } else {
        if (leftType != "Number" || rightType != "Number") {
            errors.emplace_back("Operandos de " + node.op + " deben ser números", node.line());
            node._type = "Error";
        }
        node._type = "Number";
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
        node._type = "Error";
        return;
    }

    if (node.initializer) {
        node.initializer->accept(*this);
        std::string initType = node.initializer->type();
        if (!node.declaredType.empty() && node.declaredType != initType) {
            errors.emplace_back("Tipo declarado no coincide con el inicializador", node.line());
            node._type = "Error";
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
        node._type = "Error";
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
            node._type = "Error";
        }

        // Verificar duplicados en el mismo let
        if (symbolTable.existsInCurrentScope(decl.name)) {
            errors.emplace_back("Variable '" + decl.name + "' ya declarada en este ámbito", node.line());
            node._type = "Error";
            continue;
        }

        // Analizar inicializador
        decl.initializer->accept(*this);
        std::string initType = decl.initializer->type();

        // Validar tipo declarado vs inferido
        if (!decl.declaredType.empty() && decl.declaredType != initType) {
            errors.emplace_back("Tipo declarado '" + decl.declaredType + "' no coincide con inicializador '" + initType + "'", node.line());
            node._type = "Error";
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
        node._type = "Error";
        return;
    }

    if (!symbol) {
        errors.emplace_back("Variable '" + node.name + "' no declarada", node.line());
        node._type = "Error";
        return;
    }

    if (symbol->is_const) {
        errors.emplace_back("No se puede reasignar la constante '" + node.name + "'", node.line());
        node._type = "Error";
    }

    node.rhs->accept(*this);
    std::string rhsType = node.rhs->type();

    if (rhsType != symbol->type) {
        errors.emplace_back("Tipo incorrecto en asignación: esperado '" + symbol->type + "', obtenido '" + rhsType + "'", node.line());
        node._type = "Error";
    }

    node._type = symbol->type; // Tipo de la expresión es el de la variable
}

void SemanticAnalyzer::visit(IfNode& node) {
    // 1. Verificar que existe else
    if (!node.elseBody) {
        errors.emplace_back("La expresión if debe tener cláusula else", node.line());
        node._type = "Error";
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
            // node._type = "Error";
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
                // node._type = "Error";
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
        node._type = "Error";
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
        node._type = "Error";
        return;
    }

    symbolTable.enterScope();
    symbolTable.addSymbol(node.varName, "Number", false);

    node.body->accept(*this);
    node._type = node.body->type();

    symbolTable.exitScope();
}


void SemanticAnalyzer::visit(TypeDeclarationNode& node) {
    // 1. Verificar si ya existe
    if (symbolTable.lookupType(node.name)) {
        errors.emplace_back("Tipo '" + node.name + "' ya declarado", node.line());
        return;
    }

    // 2. Verificar que no herede de tipos prohibidos
    const std::set<std::string> builtinTypes = {"Number", "String", "Boolean"};
    if (node.baseType.has_value() && builtinTypes.count(*node.baseType)) {
        errors.emplace_back("No se puede heredar de tipo básico '" + *node.baseType + "'", node.line());
        return;
    }

    // 3. Registrar tipo
    std::string parent = node.baseType.has_value() ? *node.baseType : "Object";
    std::vector<std::string> paramNames;
    for (const auto& param : *node.constructorParams) {
        paramNames.push_back(param.name);
    }

    if (!symbolTable.addType(node.name, parent, paramNames)) {
        errors.emplace_back("Tipo '" + node.name + "' ya fue registrado", node.line());
        return;
    }

    // 4. Validar y evaluar baseArgs en el contexto de los parámetros del tipo actual
    if (node.baseType.has_value()) {
        symbolTable.enterScope();
        for (const auto& param : *node.constructorParams) {
            symbolTable.addSymbol(param.name, "Unknown", false);
        }
        for (ASTNode* arg : node.baseArgs) {
            arg->accept(*this);
        }
        symbolTable.exitScope();
    }

    // 5. Evaluar inicialización de atributos sin acceso a self
    symbolTable.enterScope();
    for (const auto& param : *node.constructorParams) {
        symbolTable.addSymbol(param.name, "Unknown", false);  // Se puede mejorar con inferencia
    }

    for (const auto& attr : *node.attributes) {
        attr.initializer->accept(*this);
        symbolTable.addTypeAttribute(node.name, attr.name, attr.initializer->type());
    }
    symbolTable.exitScope();

    // 6. Evaluar métodos con acceso a self
    TypeSymbol* typeSym = symbolTable.lookupType(node.name);
    for (const auto& method : *node.methods) {
        symbolTable.enterScope();
        symbolTable.addSymbol("self", node.name, true);
        for (const auto& param : *method.params) {
            symbolTable.addSymbol(param.name, param.type, false);
        }

        method.body->accept(*this);

        // Registrar método
        std::vector<std::string> paramTypes;
        for (const auto& param : *method.params) {
            paramTypes.push_back(param.type);
        }
        symbolTable.addTypeMethod(node.name, method.name, method.returnType, paramTypes);

        // Verificar herencia de firma si aplica
        if (!typeSym->parentType.empty()) {
            TypeSymbol* parentSym = symbolTable.lookupType(typeSym->parentType);
            if (parentSym) {
                auto it = parentSym->methods.find(method.name);
                if (it != parentSym->methods.end()) {
                    const Symbol& inherited = it->second;
                    if (inherited.params != paramTypes || inherited.type != method.returnType) {
                        errors.emplace_back("Firma de método '" + method.name +
                            "' no coincide con la del padre", node.line());
                    }
                }
            }
        }

        symbolTable.exitScope();
    }
}

void SemanticAnalyzer::visit(NewInstanceNode& node) {
    TypeSymbol* typeSym = symbolTable.lookupType(node.typeName);
    if (!typeSym) {
        errors.emplace_back("Tipo '" + node.typeName + "' no definido", node.line());
        node._type = "Error";
        return;
    }

    if (node.args.size() != typeSym->typeParams.size()) {
        errors.emplace_back("Cantidad incorrecta de argumentos para el constructor de '" + node.typeName + "'", node.line());
        node._type = "Error";
        return;
    }

    for (ASTNode* arg : node.args) {
        arg->accept(*this);
        // Aquí podrías validar tipos si se almacenan los tipos de parámetros
    }

    node._type = node.typeName;
}

void SemanticAnalyzer::visit(MethodCallNode& node) {
    node.object->accept(*this);
    std::string objectType = node.object->type();

    TypeSymbol* typeSym = symbolTable.lookupType(objectType);
    if (!typeSym) {
        errors.emplace_back("Tipo '" + objectType + "' no definido para llamada a método", node.line());
        node._type = "Error";
        return;
    }

    auto it = typeSym->methods.find(node.methodName);
    if (it == typeSym->methods.end()) {
        errors.emplace_back("Método '" + node.methodName + "' no existe en tipo '" + objectType + "'", node.line());
        node._type = "Error";
        return;
    }

    Symbol& method = it->second;
    if (node.args.size() != method.params.size()) {
        errors.emplace_back("Cantidad incorrecta de argumentos en llamada a método '" + node.methodName + "'", node.line());
        node._type = "Error";
        return;
    }

    for (size_t i = 0; i < node.args.size(); ++i) {
        node.args[i]->accept(*this);
        if (node.args[i]->type() != method.params[i]) {
            errors.emplace_back("Tipo de argumento " + std::to_string(i + 1) + " incorrecto en llamada a método '" + node.methodName + "'", node.line());
        }
    }

    node._type = method.type;
}

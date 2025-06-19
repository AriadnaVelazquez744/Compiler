#include "SemanticAnalyzer.hpp"
#include "../ast/AST.hpp"
#include "FunctionCollector.hpp"
#include <cctype>
#include <set> 
#include <iostream>
#include <unordered_set>
#include <vector>
#include <algorithm>


SymbolTable& SemanticAnalyzer::getSymbolTable() {
    return symbolTable;
}

std::string SemanticAnalyzer::inferParamUsageType(const std::string& paramName, ASTNode* body) {
    std::set<std::string> usageTypes;
    collectParamUsages(body, paramName, usageTypes);

    if (usageTypes.empty()) return "Unknown";

    // Si solo hay un tipo, ese es el tipo más específico
    if (usageTypes.size() == 1) {
        return *usageTypes.begin();
    }

    // Si hay múltiples tipos, necesitamos encontrar el más específico común
    std::vector<std::string> types(usageTypes.begin(), usageTypes.end());
    
    bool allUnknown = true;
    for (const auto& type : types) {
        if (type != "Unknown") {
            allUnknown = false;
            break;
        }
    }
    if (allUnknown) {
        return "Number";
    }

    // Filtrar tipos Unknown
    types.erase(
        std::remove_if(types.begin(), types.end(),
            [](const std::string& t) { return t == "Unknown"; }),
        types.end()
    );

    if (types.empty()) {
        return "Number";
    }

    std::string commonType = symbolTable.lowestCommonAncestor(types);

    // Verificar si hay ambigüedad en la jerarquía de tipos
    bool hasAmbiguity = false;
    for (const auto& type : types) {
        if (type != commonType && !symbolTable.isSubtype(type, commonType)) {
            hasAmbiguity = true;
            break;
        }
    }

    // Si hay ambigüedad, intentamos ser más específicos
    if (hasAmbiguity) {
        // Si hay Number entre los tipos, preferimos Number
        if (std::find(types.begin(), types.end(), std::string("Number")) != types.end()) {
            return "Number";
        }
        // Si hay String entre los tipos, preferimos String
        if (std::find(types.begin(), types.end(), std::string("String")) != types.end()) {
            return "String";
        }
        // Si hay Boolean entre los tipos, preferimos Boolean
        if (std::find(types.begin(), types.end(), std::string("Boolean")) != types.end()) {
            return "Boolean";
        }
        return "Object"; // Si no podemos ser más específicos, usamos Object
    }

    return commonType;
}


void SemanticAnalyzer::collectParamUsages(ASTNode* node, const std::string& paramName, std::set<std::string>& types) {
    if (!node) return;

    // Identificador que referencia el parámetro
    if (auto* id = dynamic_cast<IdentifierNode*>(node)) {
        if (id->name == paramName && id->type() != "Unknown" && id->type() != "Error") {
            types.insert(id->type());
        }
    }

    // Binary operation - verificar restricciones de tipo
    else if (auto* bin = dynamic_cast<BinaryOpNode*>(node)) {
        collectParamUsages(bin->left, paramName, types);
        collectParamUsages(bin->right, paramName, types);
        
        // Verificar restricciones de tipo para operaciones específicas
        if (bin->op == "+" || bin->op == "-" || bin->op == "*" || bin->op == "/" || bin->op == "^"|| bin->op == "%") {
            // Operaciones aritméticas requieren Number
            auto* leftId = dynamic_cast<IdentifierNode*>(bin->left);
            auto* rightId = dynamic_cast<IdentifierNode*>(bin->right);
            if ((leftId && leftId->name == paramName) || (rightId && rightId->name == paramName)) {
                types.insert("Number");
            }
        }
        else if (bin->op == ">" || bin->op == "<" || bin->op == ">=" || bin->op == "<=") {
            // Operaciones de comparación requieren Number
            auto* leftId = dynamic_cast<IdentifierNode*>(bin->left);
            auto* rightId = dynamic_cast<IdentifierNode*>(bin->right);
            if ((leftId && leftId->name == paramName) || (rightId && rightId->name == paramName)) {
                types.insert("Number");
            }
        }
        else if (bin->op == "&" || bin->op == "|") {
            // Operaciones lógicas requieren Boolean
            auto* leftId = dynamic_cast<IdentifierNode*>(bin->left);
            auto* rightId = dynamic_cast<IdentifierNode*>(bin->right);
            if ((leftId && leftId->name == paramName) || (rightId && rightId->name == paramName)) {
                types.insert("Boolean");
            }
        }
        else if (bin->op == "@" || bin->op == "@@") {
            // Operaciones de concatenación requieren String o Number
            auto* leftId = dynamic_cast<IdentifierNode*>(bin->left);
            auto* rightId = dynamic_cast<IdentifierNode*>(bin->right);
            
            if (leftId && leftId->name == paramName) {
                // Si el otro operando es un literal, usamos su tipo
                if (auto* rightLit = dynamic_cast<LiteralNode*>(bin->right)) {
                    types.insert(rightLit->type());
                } else {
                    // Si no podemos inferir el tipo del otro operando,
                    // permitimos tanto String como Number
                    types.insert("String");
                    types.insert("Number");
                }
            }
            if (rightId && rightId->name == paramName) {
                // Si el otro operando es un literal, usamos su tipo
                if (auto* leftLit = dynamic_cast<LiteralNode*>(bin->left)) {
                    types.insert(leftLit->type());
                } else {
                    // Si no podemos inferir el tipo del otro operando,
                    // permitimos tanto String como Number
                    types.insert("String");
                    types.insert("Number");
                }
            }
        }
        else if (bin->op == "==" || bin->op == "!=") {
            // Operaciones de comparación requieren tipos compatibles
            auto* leftId = dynamic_cast<IdentifierNode*>(bin->left);
            auto* rightId = dynamic_cast<IdentifierNode*>(bin->right);
            if (leftId && leftId->name == paramName) {
                types.insert(bin->right->type());
            }
            if (rightId && rightId->name == paramName) {
                types.insert(bin->left->type());
            }
        }
    }
    // Unary operation
    else if (auto* un = dynamic_cast<UnaryOpNode*>(node)) {
        collectParamUsages(un->operand, paramName, types);
        auto* operandId = dynamic_cast<IdentifierNode*>(un->operand);
        
        if (operandId && operandId->name == paramName) {
            if (un->op == "!") {
                types.insert("Boolean");
            }
        }
    }

    // Function call
    else if (auto* call = dynamic_cast<FunctionCallNode*>(node)) {
        for (auto* arg : call->args)
            collectParamUsages(arg, paramName, types);
            
        // Si el parámetro es uno de los argumentos, inferir del tipo esperado por la función
        Symbol* funcSym = symbolTable.lookup(call->funcName);
        if (funcSym && funcSym->kind == "function") {
            for (size_t i = 0; i < call->args.size(); ++i) {
                if (auto* id = dynamic_cast<IdentifierNode*>(call->args[i])) {
                    if (id->name == paramName) {
                        // Si el tipo del parámetro de la función es Unknown, intentar inferirlo del cuerpo
                        if (funcSym->params[i] == "Unknown" && funcSym->body) {
                            std::set<std::string> paramTypes;
                            collectParamUsages(funcSym->body, funcSym->params[i], paramTypes);
                            if (!paramTypes.empty()) {
                                types.insert(*paramTypes.begin());
                            } else {
                                // Si no podemos inferir del cuerpo, intentar inferir del tipo de retorno
                                if (funcSym->type != "Unknown") {
                                    types.insert(funcSym->type);
                                }
                            }
                        } else {
                            types.insert(funcSym->params[i]);
                        }
                    }
                }
            }
        }
    }

    // Built-in function call
    else if (auto* builtin = dynamic_cast<BuiltInFunctionNode*>(node)) {
        for (auto* arg : builtin->args) {
            collectParamUsages(arg, paramName, types);
            
            // Si este argumento es el parámetro que estamos inferiendo y la función requiere Number, inferir Number
            if (auto* id = dynamic_cast<IdentifierNode*>(arg)) {
                if (id->name == paramName) {
                    static const std::set<std::string> numericBuiltins = {
                        "sin", "cos", "exp", "sqrt", "log", "min", "max"
                    };
                    if (numericBuiltins.count(builtin->name)) {
                        types.insert("Number");
                    }
                }
            }
        }
    }

    //Method call
    else if (auto* method = dynamic_cast<MethodCallNode*>(node)) {
        // Analizar todos los argumentos de la llamada al método
        for (auto* arg : method->args) {
            collectParamUsages(arg, paramName, types);
        }

        // Obtener el tipo del objeto 
        std::string objType;

        // Buscamos el símbolo de la instancia para obtener su tipo
        Symbol* instanceSym = symbolTable.lookup(method->instanceName);
        if (!instanceSym) return; 

        objType = instanceSym->type;
        TypeSymbol* typeSym = symbolTable.lookupType(objType);

        // Verificar herencia si es necesario
        while (typeSym) {
            auto it = typeSym->methods.find(method->methodName);
            if (it != typeSym->methods.end()) {
                const Symbol& methodSym = it->second;

                for (size_t i = 0; i < method->args.size(); ++i) {
                    if (auto* id = dynamic_cast<IdentifierNode*>(method->args[i])) {
                        if (id->name == paramName && i < methodSym.params.size()) {
                            types.insert(methodSym.params[i]);
                        }
                    }
                }

                break; // método encontrado, no seguimos subiendo en herencia
            }

            if (typeSym->parentType.empty()) break;
            typeSym = symbolTable.lookupType(typeSym->parentType);
        }
    }

    // Let expression
    else if (auto* let = dynamic_cast<LetNode*>(node)) {
        // Primero analizar las declaraciones
        for (auto& decl : *let->declarations) {
            collectParamUsages(decl.initializer, paramName, types);
            
            // Si el parámetro es usado en el inicializador, su tipo debe ser compatible
            if (auto* id = dynamic_cast<IdentifierNode*>(decl.initializer)) {
                if (id->name == paramName) {
                    types.insert(decl.declaredType);
                }
            }
        }
        collectParamUsages(let->body, paramName, types);
    }

    // If expression
    else if (auto* ifn = dynamic_cast<IfNode*>(node)) {
        // Analizar condición
        collectParamUsages(ifn->branches->front().condition, paramName, types);
        
        // Analizar todas las ramas
        for (auto& branch : *ifn->branches) {
            collectParamUsages(branch.body, paramName, types);
        }
        
        // Analizar else si existe
        if (ifn->elseBody) {
            collectParamUsages(ifn->elseBody, paramName, types);
        }
    }

    // While expression
    else if (auto* wh = dynamic_cast<WhileNode*>(node)) {
        collectParamUsages(wh->condition, paramName, types);
        collectParamUsages(wh->body, paramName, types);
    }

    // For expression
    else if (auto* forNode = dynamic_cast<ForNode*>(node)) {
        collectParamUsages(forNode->init_range, paramName, types);
        collectParamUsages(forNode->end_range, paramName, types);
        collectParamUsages(forNode->body, paramName, types);
    }

    //Assignment
    else if (auto* assign = dynamic_cast<AssignmentNode*>(node)) {
        collectParamUsages(assign->rhs, paramName, types);

    if (auto* id = dynamic_cast<IdentifierNode*>(assign->rhs)) {
        if (id->name == paramName) {
            std::string targetName;

            if (auto* idName = dynamic_cast<IdentifierNode*>(assign->name)) {
                targetName = idName->name;
            } else if (auto* selfField = dynamic_cast<SelfCallNode*>(assign->name)) {
                targetName = selfField->varName;
            }

            if (!targetName.empty()) {
                Symbol* varSym = symbolTable.lookup(targetName);
                if (varSym) {
                    types.insert(varSym->type);
                }
            }
        }
    }
    }

    // Variable declaration
    else if (auto* decl = dynamic_cast<VariableDeclarationNode*>(node)) {
        if (decl->initializer) {
            collectParamUsages(decl->initializer, paramName, types);
            
            // Si el parámetro es usado en el inicializador, su tipo debe ser compatible
            if (auto* id = dynamic_cast<IdentifierNode*>(decl->initializer)) {
                if (id->name == paramName) {
                    types.insert(decl->declaredType);
                }
            }
        }
    }

    // Block
    else if (auto* block = dynamic_cast<BlockNode*>(node)) {
        for (auto* expr : block->expressions)
            collectParamUsages(expr, paramName, types);
    }

    // New instance
    else if (auto* inst = dynamic_cast<NewInstanceNode*>(node)) {
        for (auto* arg : inst->args)
            collectParamUsages(arg, paramName, types);
            
        // Si el parámetro es uno de los argumentos, inferir del tipo esperado por el constructor
        TypeSymbol* typeSym = symbolTable.lookupType(inst->typeName);
        if (typeSym) {
            for (size_t i = 0; i < inst->args.size(); ++i) {
                if (auto* id = dynamic_cast<IdentifierNode*>(inst->args[i])) {
                    if (id->name == paramName && i < typeSym->typeParams.size()) {
                        types.insert(typeSym->typeParams[i]);
                    }
                }
            }
        }
    }

    // Type declaration
    else if (auto* typeDecl = dynamic_cast<TypeDeclarationNode*>(node)) {
         // Recolectar tipos desde atributos
        if (typeDecl->body && typeDecl->body->attributes) {
            for (const auto& attr : *typeDecl->body->attributes) {
                collectParamUsages(attr.initializer, paramName, types);
            }
        }

        // Recolectar tipos desde cuerpos de métodos
        if (typeDecl->body && typeDecl->body->methods) {
            for (const auto& method : *typeDecl->body->methods) {
                collectParamUsages(method.body, paramName, types);
            }
        }

        // Argumentos al padre (base constructor args)
        for (ASTNode* arg : typeDecl->baseArgs) {
            collectParamUsages(arg, paramName, types);
        }
    }

    // Literal: no hay nada que recorrer
    else if (dynamic_cast<LiteralNode*>(node)) {
        return;
    }

    // Otro nodo no manejado explícitamente
    else {
        // std::cerr << "Nodo no manejado en collectParamUsages: " << typeid(*node).name() << "\n";
    }
}

// std::string SemanticAnalyzer::lowestCommonAncestor(const std::set<std::string>& types) {
//     if (types.empty()) return "Object";
//     if (types.size() == 1) return *types.begin();

//     std::vector<std::string> all(types.begin(), types.end());
//     std::string ancestor = all[0];
//     for (size_t i = 1; i < all.size(); ++i) {
//         ancestor = commonAncestor(ancestor, all[i]);
//         if (ancestor == "Object") break;
//     }
//     return ancestor;
// }

// std::string SemanticAnalyzer::commonAncestor(const std::string& t1, const std::string& t2) {
//     std::set<std::string> ancestors;
//     std::string current = t1;
//     while (!current.empty()) {
//         ancestors.insert(current);
//         TypeSymbol* type = symbolTable.lookupType(current);
//         if (!type) break;
//         current = type->parentType;
//     }

//     current = t2;
//     while (!current.empty()) {
//         if (ancestors.count(current)) return current;
//         TypeSymbol* type = symbolTable.lookupType(current);
//         if (!type) break;
//         current = type->parentType;
//     }
//     return "Object";
// }




// void SemanticAnalyzer::resolveFunctionTypes() {
//     auto functions = symbolTable.getUserDefinedFunctions();

//     for (Symbol& symbol : functions) {
//         if (!symbol.body) continue;

//         symbolTable.enterScope();

//         std::vector<std::string> paramNames;
//         std::vector<std::string>& paramTypes = symbol.params;

//         for (size_t i = 0; i < paramTypes.size(); ++i) {
//             std::string name = "param" + std::to_string(i);
//             paramNames.push_back(name);
//             std::string type = paramTypes[i].empty() ? "Unknown" : paramTypes[i];
//             symbolTable.addSymbol(name, type, false);
//         }

//         symbol.body->accept(*this);
//         std::string bodyType = symbol.body->type();

//         if (symbol.type.empty() || symbol.type == "Unknown") {
//             symbol.type = bodyType;
//         }

//         for (size_t i = 0; i < paramTypes.size(); ++i) {
//             Symbol* paramSym = symbolTable.lookup(paramNames[i]);
//             if (paramSym && (paramTypes[i].empty() || paramTypes[i] == "Unknown")) {
//                 paramTypes[i] = paramSym->type;
//             }
//         }

//         symbolTable.exitScope();
//     }
// }



bool SemanticAnalyzer::conformsTo(const std::string& subtype, const std::string& supertype) {
    if (subtype == "Error" || supertype == "Error") return false;
    if (subtype == supertype) return true;
    if (supertype == "Object") return true;
    // if (supertype == "")
    // {
    //     return true;
    // }
    
    

    TypeSymbol* sub = symbolTable.lookupType(subtype);
    while (sub && !sub->parentType.empty()) {
        if (sub->parentType == supertype) return true;
        sub = symbolTable.lookupType(sub->parentType);
    }

    return false;
}

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

    // SemanticAnalyzer analyzer(symbolTable, errors);
    // analyzer.resolveFunctionTypes();    

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
    node.operand->accept(*this);
    std::string operandType = node.operand->type();

    if (node.op == "-") {
        if (operandType != "Number") {
            errors.emplace_back("El operador '-' requiere un operando de tipo Number", node.line());
            node._type = "Error";
        }
        node._type = "Number";
    } else if (node.op == "!") {
        
        if (operandType == "Unknown") operandType = "Boolean";
        
        if (operandType != "Boolean") {
            errors.emplace_back("El operador '!' requiere un operando de tipo Boolean", node.line());
            node._type = "Error";
        } else {
            node._type = "Boolean";
        }
    } else {
        errors.emplace_back("Operador unario desconocido: " + node.op, node.line());
        node._type = "Error";
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
        
        }
        if (node.args[0]->type() == "Unknown"){
            node.args[0]->type() = "Number";
        } 
         
        else if (node.args[0]->type() != "Number") {
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
        }if (node.args[0]->type() == "Unknown"){
            node.args[0]->type() = "Number";
        }
        if (node.args[1]->type() == "Unknown") node.args[1]->type() = "Number";
        
        else if (node.args[0]->type() != "Number" || node.args[1]->type() != "Number") {
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
        }if (node.args[0]->type() == "Unknown"){
            node.args[0]->type() = "Number";
        }
        if (node.args[1]->type() == "Unknown") node.args[1]->type() = "Number";
         else if (node.args[0]->type() != "Number" || node.args[1]->type() != "Number") {
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
    std::cout << "\n=== Iniciando análisis de función: " << node.name << " ===\n";
    // Paso 0: Pre-registro provisional en la tabla de símbolos
    std::vector<std::string> paramTypes;
    for (const auto& param : *node.params)
        paramTypes.push_back(param.type.empty() ? "Unknown" : param.type);

    symbolTable.addFunction(node.name, "Unknown", paramTypes); // tipo de retorno provisional

    symbolTable.enterScope();

    std::unordered_map<std::string, bool> paramSeen;

    // Paso 1: insertar todos los parámetros con tipo Unknown (si no están anotados)
    std::cout << "Paso 1: Registrando parámetros\n";
    for (const auto& param : *node.params) {
        if (paramSeen.count(param.name)) {
            errors.emplace_back("Parámetro duplicado '" + param.name + "'", node.line());
            node._type = "Error";
            continue;
        }

        paramSeen[param.name] = true;
        std::string paramType = param.type.empty() ? "Unknown" : param.type;
        std::cout << "  - Parámetro: " << param.name << " con tipo inicial: " << paramType << "\n";
        symbolTable.addSymbol(param.name, paramType, false);
    }

    // Paso 2: analizar el cuerpo con símbolos disponibles
    std::cout << "Paso 2: Analizando cuerpo de la función\n";
    node.body->accept(*this);
    std::string bodyType = node.body->type();
    std::cout << "  - Tipo del cuerpo: " << bodyType << "\n";

    // Paso 3: Inferencia de tipos para parámetros sin tipo explícito
    std::cout << "DEBUG: Iniciando inferencia de tipos para parámetros\n";
    for (auto& param : *node.params) {
        if (!param.type.empty()) {
            continue;
        }

        std::cout << "  - Intentando inferir tipo para parámetro: " << param.name << "\n";

        // Si el cuerpo es una llamada a función, verificar el tipo esperado por la función
        if (auto* call = dynamic_cast<FunctionCallNode*>(node.body)) {
            Symbol* funcSym = symbolTable.lookup(call->funcName);
            if (funcSym && funcSym->kind == "function") {
                for (size_t i = 0; i < call->args.size(); ++i) {
                    if (auto* id = dynamic_cast<IdentifierNode*>(call->args[i])) {
                        if (id->name == param.name) {
                            std::string expectedType = funcSym->params[i];
                            if (expectedType != "Unknown") {
                                param.type = expectedType;
                                symbolTable.updateSymbolType(param.name, expectedType);
                                std::cout << "    - Tipo inferido de llamada a función: " << expectedType << "\n";
                                continue;
                            }
                        }
                    }
                }
            }
        }

        // Si el cuerpo es una operación binaria, verificar restricciones de operadores
        if (auto* bin = dynamic_cast<BinaryOpNode*>(node.body)) {
            std::cout << "    - Cuerpo es una operación binaria con operador: " << bin->op << "\n";
            if (bin->op == "+" || bin->op == "-" || bin->op == "*" || bin->op == "/") {
                auto* leftId = dynamic_cast<IdentifierNode*>(bin->left);
                auto* rightId = dynamic_cast<IdentifierNode*>(bin->right);
                
                std::cout << "    - Verificando operandos:\n";
                if (leftId) std::cout << "      - Izquierdo: " << leftId->name << "\n";
                if (rightId) std::cout << "      - Derecho: " << rightId->name << "\n";
                
                if ((leftId && leftId->name == param.name) || 
                    (rightId && rightId->name == param.name)) {
                    std::cout << "    - Parámetro usado en operación aritmética, asignando tipo Number\n";
                    param.type = "Number";
                    symbolTable.updateSymbolType(param.name, "Number");
                    continue;
                }
            }
            else if (bin->op == "@" || bin->op == "@@") {
                auto* leftId = dynamic_cast<IdentifierNode*>(bin->left);
                auto* rightId = dynamic_cast<IdentifierNode*>(bin->right);
                
                if ((leftId && leftId->name == param.name) || 
                    (rightId && rightId->name == param.name)) {
                    // Si el otro operando es un literal, usar su tipo
                    if (auto* otherLit = dynamic_cast<LiteralNode*>(leftId ? bin->right : bin->left)) {
                        param.type = otherLit->type();
                    } else {
                        // Si no podemos inferir el tipo del otro operando,
                        // permitimos tanto String como Number
                        param.type = "String";
                    }
                    symbolTable.updateSymbolType(param.name, param.type);
                    std::cout << "    - Parámetro usado en concatenación, asignando tipo: " << param.type << "\n";
                    continue;
                }
            }
        }

        // Si no es una operación binaria o no se pudo inferir por operador,
        // usar el método general de inferencia
        std::cout << "    - Usando método general de inferencia\n";
        std::string inferredType = inferParamUsageType(param.name, node.body);
        std::cout << "    - Tipo inferido: " << inferredType << "\n";

        if (inferredType == "Unknown") {
            // Si no se pudo inferir, intentar inferir del tipo de retorno
            if (bodyType != "Unknown" && bodyType != "Error") {
                inferredType = bodyType;
                std::cout << "    - Usando tipo de retorno como tipo inferido: " << inferredType << "\n";
            } else {
                errors.emplace_back("No se pudo inferir el tipo del parámetro '" + param.name + "'", node.line());
                node._type = "Error";
                continue;
            }
        }

        param.type = inferredType;
        symbolTable.updateSymbolType(param.name, inferredType);
    }

    // Paso 4: Verificación de tipo de retorno
    std::cout << "Paso 4: Verificando tipo de retorno\n";
    if (!node.returnType.empty() && !conformsTo(bodyType, node.returnType)) {
        errors.emplace_back("Tipo de retorno incorrecto en función '" + node.name + "'", node.line());
        node._type = "Error";
    }

    node._type = node.returnType.empty() ? bodyType : node.returnType;
    std::cout << "  - Tipo final de la función: " << node._type << "\n";

    // Actualizar el tipo de retorno en la tabla de símbolos
    Symbol* funcSym = symbolTable.lookup(node.name);
    if (funcSym) {
        funcSym->type = node._type;
    }

    std::cout << "=== Fin del análisis de función ===\n\n";

    symbolTable.exitScope();
}

void SemanticAnalyzer::visit(FunctionCallNode& node) {
    if (node.funcName == "base") {
        Symbol* self = symbolTable.lookup("self");
        if (!self) {
            errors.emplace_back("'base' solo puede usarse dentro de métodos", node.line());
            node._type = "Error";
            return;
        }

        TypeSymbol* typeSym = symbolTable.lookupType(self->type);
        if (!typeSym || typeSym->parentType == "Object") {
            errors.emplace_back("'base' no disponible para este tipo", node.line());
            node._type = "Error";
            return;
        }

        node._type = typeSym->parentType;
        return;
    }

    // Funciones built-in como 'print'
    if (node.funcName == "print") {
        for (auto arg : node.args) {
            arg->accept(*this);
        }
        node._type = "void";
        return;
    }

    // Funciones definidas por el usuario
    Symbol* symbol = symbolTable.lookup(node.funcName);
    if (!symbol || symbol->kind != "function") {
        errors.emplace_back("Función '" + node.funcName + "' no definida", node.line());
        node._type = "Error";
        return;
    }

    if (node.args.size() != symbol->params.size()) {
        errors.emplace_back("Número incorrecto de argumentos para '" + node.funcName + "'", node.line());
        node._type = "Error";
        return;
    }

    // Primero analizar todos los argumentos
    for (size_t i = 0; i < node.args.size(); ++i) {
        node.args[i]->accept(*this);
        std::string argType = node.args[i]->type();
        std::string expectedType = symbol->params[i];

        if (expectedType == "")
        {
            expectedType = "Unknown";
        }

        // Si el tipo esperado es Unknown, intentar inferirlo del cuerpo de la función
        if ((expectedType == "Unknown" || expectedType == "") && symbol->body) {
            std::set<std::string> paramTypes;
            collectParamUsages(symbol->body, symbol->params[i], paramTypes);
            if (!paramTypes.empty()) {
                expectedType = *paramTypes.begin();
                symbol->params[i] = expectedType; // Actualizar el tipo en la tabla de símbolos
            }
        }

        // Si el tipo del argumento es Unknown, intentar inferirlo del tipo esperado
        if (argType == "Unknown" && expectedType != "Unknown") {
            argType = expectedType;
            node.args[i]->type() = expectedType;
        }

        // Si ambos tipos son Unknown, intentar inferir del uso en el cuerpo
        if (argType == "Unknown" && expectedType == "Unknown" && symbol->body) {
            std::set<std::string> paramTypes;
            collectParamUsages(symbol->body, symbol->params[i], paramTypes);
            if (!paramTypes.empty()) {
                expectedType = *paramTypes.begin();
                symbol->params[i] = expectedType;
                argType = expectedType;
                node.args[i]->type() = expectedType;
            }
        }

        std::cout << "DEBUG: argType=" << argType << ", expectedType=" << expectedType << std::endl;

        // Verificar compatibilidad de tipos
        if (!conformsTo(argType, expectedType)) {
            std::cout << "DEBUUG: argType=" << argType << ", expectedType=" << expectedType << std::endl;
            // Si el tipo esperado es Unknown, intentar inferir del argumento
            if ((expectedType == "Unknown" || expectedType == "") && argType != "Unknown") {
                symbol->params[i] = argType;
            } else {
                errors.emplace_back("Tipo incorrecto para argumento " + std::to_string(i + 1) +
                    " en '" + node.funcName + "': esperado '" + expectedType + "', obtenido '" + argType + "'",
                    node.line());
                node._type = "Error";
                return;
            }
        }
    }

    // El tipo de la llamada es el tipo de retorno de la función
    node._type = symbol->type;
    
    // Si el tipo de retorno es Unknown, intentar inferirlo del cuerpo
    if (node._type == "Unknown" && symbol->body) {
        symbol->body->accept(*this);
        node._type = symbol->body->type();
        symbol->type = node._type; // Actualizar el tipo en la tabla de símbolos
    }
}


void SemanticAnalyzer::visit(BinaryOpNode& node) {
    node.left->accept(*this);
    node.right->accept(*this);

    std::string leftType = node.left->type();
    std::string rightType = node.right->type();

    const std::set<std::string> comparisonOps = {"==", "!=", "<", ">", "<=", ">="};
    const std::set<std::string> arithmeticOps = {"+", "-", "*", "/", "^", "%"};

    if (comparisonOps.count(node.op)) {
        // Para comparaciones, si alguno es Unknown, intentamos inferir del otro
        if (leftType == "Unknown") {
            if (rightType == "Number" || rightType == "String" || rightType == "Boolean") {
                leftType = rightType;
            }
        }
        if (rightType == "Unknown") {
            if (leftType == "Number" || leftType == "String" || leftType == "Boolean") {
                rightType = leftType;
            }
        }

        if (node.op == "==" || node.op == "!=") {
            if (!conformsTo(leftType, rightType) && !conformsTo(rightType, leftType)) {
                errors.emplace_back("Operandos de " + node.op + " deben ser compatibles", node.line());
                node._type = "Error";
            } else {
                node._type = "Boolean";
            }
        } else {
            // Para <, >, <=, >=, ambos operandos deben ser Number
            if (leftType == "Unknown") leftType = "Number";
            if (rightType == "Unknown") rightType = "Number";
            
            if (leftType != "Number" || rightType != "Number") {
                errors.emplace_back("Operandos de " + node.op + " deben ser números", node.line());
                node._type = "Error";
            } else {
                node._type = "Boolean";
            }
        }
    }
    else if (arithmeticOps.count(node.op)) {
        // Si alguno de los operandos es Unknown, asumimos que es Number
        // if (node.op == "+")
        // {
        //     std::cout << leftType << std::endl;
        //     std::cout << "Hola" << std::endl;
        //     std::cout << rightType << std::endl;
        // }
        
        if (leftType == "Unknown" || leftType == "") leftType = "Number";
        if (rightType == "Unknown"|| rightType == "") rightType = "Number";
        
        if (leftType != "Number" || rightType != "Number") {
            errors.emplace_back("Operandos de " + node.op + " deben ser números", node.line());
            node._type = "Error";
        } else {
            node._type = "Number";
        }
    }
    else if (node.op == "&" || node.op == "|") {
        // Para operadores lógicos, si alguno es Unknown, asumimos Boolean
        if (leftType == "Unknown") leftType = "Boolean";
        if (rightType == "Unknown") rightType = "Boolean";
        
        if (leftType != "Boolean" || rightType != "Boolean") {
            errors.emplace_back("Operandos de " + node.op + " deben ser booleanos", node.line());
            node._type = "Error";
        } else {
            node._type = "Boolean";
        }
    }
    else if (node.op == "@" || node.op == "@@") {
        // Si alguno de los operandos es Unknown, permitimos tanto String como Number
        // if (node.op == "@")
        // {
        //     std::cout << leftType << std::endl;
        //     std::cout << "Ho" << std::endl;
        //     std::cout << rightType << std::endl;
        // }
        
        if (leftType == "Unknown" || leftType == "") {
            if (rightType == "String" || rightType == "Number") {
                leftType = rightType;
            } else if (rightType == "Unknown"|| rightType == "") {
                leftType = "String"; // Preferimos String para concatenación
                rightType = "String";
            }
        }
        if (rightType == "Unknown"|| rightType == "") {
            if (leftType == "String" || leftType == "Number") {
                rightType = leftType;
            }
        }

        if ((leftType != "String" && leftType != "Number") ||
            (rightType != "String" && rightType != "Number")) {
            errors.emplace_back("Operandos de @ deben ser string o number", node.line());
            node._type = "Error";
        } else {
            node._type = "String";
        }
    }
    else {
        errors.emplace_back("Operador binario desconocido: " + node.op, node.line());
        node._type = "Error";
    }
}
// ------------------------------------------------------


void SemanticAnalyzer::visit(LiteralNode& node) {
    // El tipo ya está definido (number, string, boolean)
    node._type = node._type;
}

// ------------------------------------------------------

void SemanticAnalyzer::visit(BlockNode& node) {
    if (node.expressions.empty()) {
        node._type = "Null";
        return;
    }

    symbolTable.enterScope();

    // Analizar todas las expresiones
    for (auto* expr : node.expressions) {
        // Verificar que no sea una declaración de tipo
        if (dynamic_cast<TypeDeclarationNode*>(expr)) {
            errors.emplace_back("No se pueden declarar tipos dentro de bloques", expr->line());
            node._type = "Error";
            symbolTable.exitScope();
            return;
        }
        expr->accept(*this);
    }

    // Obtener la última expresión
    ASTNode* lastExpr = node.expressions.back();

    // Si la última expresión es una declaración de función o un bloque que termina con una declaración de función
    if (dynamic_cast<FunctionDeclarationNode*>(lastExpr) || 
        (dynamic_cast<BlockNode*>(lastExpr) && 
         !dynamic_cast<BlockNode*>(lastExpr)->expressions.empty() && 
         dynamic_cast<FunctionDeclarationNode*>(dynamic_cast<BlockNode*>(lastExpr)->expressions.back()))) {
        node._type = "Null";
    } else {
        // En caso contrario, el tipo del bloque es el tipo de su última expresión
        node._type = lastExpr->type();
    }

        // node._type = node.expressions.back()->type();

    symbolTable.exitScope();
}

// ------------------------------------------------------

void SemanticAnalyzer::visit(VariableDeclarationNode& node) {
    if (symbolTable.existsInCurrentScope(node.name)) {
        errors.emplace_back("Variable '" + node.name + "' ya declarada", node.line());
        node._type = "Error";
        return;
    }

    if (!node.initializer && node.declaredType.empty()) {
        errors.emplace_back("Variable '" + node.name + "' sin tipo ni inicializador", node.line());
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
    symbolTable.enterScope();

    for (auto& decl : *node.declarations) {
        // Verificar nombre válido
        if (!isValidIdentifier(decl.name)) {
            errors.emplace_back("Nombre inválido: '" + decl.name + "'", node.line());
            node._type = "Error";
            continue;
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

        // Si no hay tipo declarado, usar el tipo del inicializador
        if (decl.declaredType.empty()) {
            decl.declaredType = initType;
        } else if (!conformsTo(initType, decl.declaredType)) {
            errors.emplace_back("Tipo declarado '" + decl.declaredType + "' no coincide con inicializador '" + initType + "'", node.line());
            node._type = "Error";
            continue;
        }

        // Registrar en tabla de símbolos
        symbolTable.addSymbol(decl.name, decl.declaredType, false);
    }

    // Analizar cuerpo
    node.body->accept(*this);
    node._type = node.body->type();

    symbolTable.exitScope();
}

void SemanticAnalyzer::visit(AssignmentNode& node) {
    
    std::string name;
    if(auto* id = dynamic_cast<IdentifierNode*>(node.name)) {
        name = id->name;
    }
    else if (auto* self = dynamic_cast<SelfCallNode*>(node.name)) {
        name = self->varName;
    }

    Symbol* symbol = symbolTable.lookup(name);

    if (name == "self") {
        errors.emplace_back("No se puede reasignar 'self'", node.line());
        node._type = "Error";
        return;
    }


    if (!symbol) {
        errors.emplace_back("Variable '" + name + "' no declarada", node.line());
        node._type = "Error";
        return;
    }

    if (symbol->is_const) {
        errors.emplace_back("No se puede reasignar la constante '" + name + "'", node.line());
        node._type = "Error";
    }

    node.rhs->accept(*this);
    std::string rhsType = node.rhs->type();

    if (symbol->type == "Unknown") {
        symbolTable.updateSymbolType(name, rhsType);
        node._type = rhsType;
    } else if (!conformsTo(rhsType, symbol->type)) {
        errors.emplace_back("Tipo incorrecto en asignación: esperado '" + symbol->type + "', obtenido '" + rhsType + "'", node.line());
        node._type = "Error";
    } else {
        node._type = symbol->type;
    }
    node._type = symbol->type;
}

void SemanticAnalyzer::visit(IfNode& node) {

    std::vector<std::string> branchTypes;
    bool hasErrors = false;

    // 2. Verificar condiciones y recolectar tipos
    for (auto& branch : *node.branches) {
        // Analizar condición
        branch.condition->accept(*this);
        std::string condType = branch.condition->type();
        if (condType != "Boolean") {
            errors.emplace_back("Condición debe ser booleana", branch.condition->line());
            // node._type = "Error";
            hasErrors = true;
        }

        // Analizar cuerpo de la rama
        branch.body->accept(*this);
        branchTypes.push_back(branch.body->type());
    }

    // Procesar 'else' si está presente
    if (node.elseBody) {
        node.elseBody->accept(*this);
        branchTypes.push_back(node.elseBody->type());
    }

    if (hasErrors) {
        node._type = "Error";
        return;
    }

    // Verificar consistencia de tipos
    const std::string& commonType = branchTypes.front();
    for (const auto& t : branchTypes) {
        if (t != commonType) {
            errors.emplace_back("Tipos incompatibles en ramas del 'if'", node.line());
            node._type = "Error";
            // std::cout << t << std::endl;
            // std::cout << commonType << std::endl;
            return;

        }
    }

    node._type = commonType;
}

void SemanticAnalyzer::visit(WhileNode& node) {
    // Verificar condición es booleana
    node.condition->accept(*this);
    std::string condType = node.condition->type();
    if (condType != "Boolean") {
        errors.emplace_back("Condición del while debe ser booleana", node.line());
        node._type = "Error";
        std::cout << condType << std::endl;
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
    std::cout << "Analizando tipo: " << node.name << "\n";

    // 1. Verificar redefinición
    if (symbolTable.lookupType(node.name)) {
        errors.emplace_back("Tipo '" + node.name + "' ya está definido", node.line());
        return;
    }

    // 2. Validar que no hereda de tipos prohibidos
    const std::set<std::string> builtinTypes = {"Number", "String", "Boolean"};
    if (node.baseType.has_value() && builtinTypes.count(*node.baseType)) {
        errors.emplace_back("No se puede heredar de tipo básico '" + *node.baseType + "'", node.line());
        return;
    }

    std::string parent = node.baseType.value_or("Object");

    // 3. Registrar el tipo en la tabla
    std::vector<std::string> paramNames;
    for (const auto& param : *node.constructorParams) {
        paramNames.push_back(param.name);
    }

    if (!symbolTable.addType(node.name, parent, paramNames)) {
        errors.emplace_back("No se pudo registrar el tipo '" + node.name + "'", node.line());
        return;
    }

    // 4. Si hereda y no declara baseArgs, asumir que se pasa los parámetros propios al padre
    if (node.baseType.has_value() && node.baseArgs.empty()) {
        TypeSymbol* parentSym = symbolTable.lookupType(*node.baseType);
        if (!parentSym) {
            std::cerr << "[DEBUG] No se encontró el tipo base '" << *node.baseType << "'\n";
            errors.emplace_back("Tipo base '" + *node.baseType + "' no encontrado", node.line());
            return;
        }

        // Herencia sin constructor explícito => heredar del padre
        if (node.baseType.has_value() && node.constructorParams->empty() && node.baseArgs.empty()) {
            TypeSymbol* parentSym = symbolTable.lookupType(*node.baseType);
            if (parentSym) {
                std::cerr << "[DEBUG] Tipo '" << node.name << "' hereda de '" << *node.baseType
                        << "' sin constructor explícito. Heredando parámetros del padre...\n";
                
                for (const std::string& paramName : parentSym->typeParams) {
                    node.constructorParams->emplace_back(Parameter{paramName, ""});
                    node.baseArgs.push_back(new IdentifierNode(paramName, node.line()));
                    std::cerr << "  + Param heredado: " << paramName << "\n";
                }
            }
            symbolTable.updateTypeParams(node.name, parentSym->typeParams);
        }


        if (parentSym->typeParams.size() != node.constructorParams->size()) {
            std::cerr << "[DEBUG] Tipo padre '" << parentSym->name
                    << "' espera " << parentSym->typeParams.size()
                    << " parámetros, pero se encontraron "
                    << node.constructorParams->size()
                    << " en el tipo hijo '" << node.name << "'\n";
            errors.emplace_back("Cantidad incorrecta de argumentos para constructor del padre", node.line());
            return;
        }

        for (const auto& param : *node.constructorParams) {
            node.baseArgs.push_back(new IdentifierNode(param.name, node.line()));
        }
    }

    // 5. Analizar baseArgs (si hay)
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

    // 6. Analizar atributos (sin self)
    symbolTable.enterScope();
    for (const auto& param : *node.constructorParams) {
        symbolTable.addSymbol(param.name, "Unknown", false);
    }

    for (const auto& attr : *node.body->attributes) {
        attr.initializer->accept(*this);
        std::string attrType = attr.initializer->type();

        if (attrType == "Error" || attrType.empty()) {
            errors.emplace_back("No se pudo inferir el tipo del atributo '" + attr.name + "'", node.line());
        } else {
            symbolTable.addTypeAttribute(node.name, attr.name, attrType);
        }
    }

    symbolTable.exitScope();

    // 7. Analizar métodos
    TypeSymbol* typeSym = symbolTable.lookupType(node.name);

    for (const auto& method : *node.body->methods) {
        symbolTable.enterScope();
        symbolTable.addSymbol("self", node.name, true);

        currentMethodContext = method.name;  

        for (const auto& param : *method.params) {
            symbolTable.addSymbol(param.name, param.type, false);
        }

        method.body->accept(*this);

        // Verificación de tipo de retorno
        if (!method.returnType.empty() &&
            !conformsTo(method.body->type(), method.returnType)) {
            errors.emplace_back("El cuerpo del método '" + method.name +
                                "' no conforma al tipo de retorno declarado", method.body->line());
        }

        std::vector<std::string> paramTypes;
        for (const auto& param : *method.params) {
            paramTypes.push_back(param.type);
        }

        symbolTable.addTypeMethod(node.name, method.name, method.returnType, paramTypes);

        // Verificación de firma heredada si aplica
        if (!typeSym->parentType.empty()) {
            TypeSymbol* parentSym = symbolTable.lookupType(typeSym->parentType);
            if (parentSym) {
                auto it = parentSym->methods.find(method.name);
                if (it != parentSym->methods.end()) {
                    const Symbol& inherited = it->second;
                    if (inherited.params != paramTypes || inherited.type != method.returnType) {
                        errors.emplace_back("Firma de método '" + method.name +
                            "' no coincide con la del tipo padre '" + typeSym->parentType + "'", method.body->line());
                    }
                }
            }
        }

        currentMethodContext.clear(); 
        symbolTable.exitScope();
    }

    std::cout << "Tipo '" << node.name << "' analizado correctamente\n";
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
    Symbol* instSym = symbolTable.lookup(node.instanceName);
    if (!instSym) {
        errors.emplace_back("Variable '" + node.instanceName + "' no declarada", node.line());
        node._type = "Error";
        return;
    }

    TypeSymbol* typeSym = symbolTable.lookupType(instSym->type);
    if (!typeSym) {
        errors.emplace_back("Tipo '" + instSym->type + "' no definido", node.line());
        node._type = "Error";
        return;
    }

    // Búsqueda jerárquica del método en la cadena de herencia
    const Symbol* method = nullptr;
    while (typeSym) {
        auto it = typeSym->methods.find(node.methodName);
        if (it != typeSym->methods.end()) {
            method = &it->second;
            break;
        }
        if (typeSym->parentType.empty()) break;
        typeSym = symbolTable.lookupType(typeSym->parentType);
    }

    if (!method) {
        errors.emplace_back("Método '" + node.methodName + "' no existe en tipo '" + instSym->type + "'", node.line());
        node._type = "Error";
        return;
    }

    // Validar cantidad de argumentos
    if (node.args.size() != method->params.size()) {
        errors.emplace_back("Cantidad incorrecta de argumentos en método '" + node.methodName + "'", node.line());
        node._type = "Error";
        return;
    }

    // Verificar tipos de argumentos
    for (size_t i = 0; i < node.args.size(); ++i) {
        node.args[i]->accept(*this);
        if (!conformsTo(node.args[i]->type(), method->params[i])) {
            errors.emplace_back("Tipo incorrecto para argumento " + std::to_string(i+1) +
                                " en llamada a '" + node.methodName + "'", node.line());
        }
    }

    node._type = method->type;
}



void SemanticAnalyzer::visit(AttributeDeclaration& node) {
    
    node.initializer->accept(*this);
   
}

void SemanticAnalyzer::visit(MethodDeclaration& node) {
    
}

void SemanticAnalyzer::visit(BaseCallNode& node) {
    Symbol* self = symbolTable.lookup("self");
    if (!self) {
        errors.emplace_back("'base' solo puede usarse dentro de métodos", node.line());
        node._type = "Error";
        return;
    }

    TypeSymbol* typeSym = symbolTable.lookupType(self->type);
    if (!typeSym || typeSym->parentType.empty()) {
        errors.emplace_back("'base' no disponible para este tipo", node.line());
        node._type = "Error";
        return;
    }

    // Necesitamos saber en qué método estamos
    std::string currentMethodName = currentMethodContext;
    if (currentMethodName.empty()) {
        errors.emplace_back("'base' solo puede usarse dentro de un método con nombre", node.line());
        node._type = "Error";
        return;
    }

    // Buscar en el padre el método con el mismo nombre
    TypeSymbol* parentSym = symbolTable.lookupType(typeSym->parentType);
    if (!parentSym) {
        errors.emplace_back("Tipo padre '" + typeSym->parentType + "' no encontrado", node.line());
        node._type = "Error";
        return;
    }

    auto it = parentSym->methods.find(currentMethodName);
    if (it == parentSym->methods.end()) {
        errors.emplace_back("Método '" + currentMethodName + "' no existe en el padre '" + parentSym->name + "'", node.line());
        node._type = "Error";
        return;
    }

    node._type = it->second.type; // Usamos el tipo de retorno del método padre
}

void SemanticAnalyzer::visit(SelfCallNode& node) {
    // Check if we're in a method context
    Symbol* self = symbolTable.lookup("self");
    if (!self) {
        errors.emplace_back("'self' solo puede usarse dentro de métodos", node.line());
        node._type = "Error";
        return;
    }

    // Get the type of self
    TypeSymbol* typeSym = symbolTable.lookupType(self->type);
    if (!typeSym) {
        errors.emplace_back("Tipo de 'self' no encontrado", node.line());
        node._type = "Error";
        return;
    }

    // Check if the attribute exists
    auto it = typeSym->attributes.find(node.varName);
    if (it == typeSym->attributes.end()) {
        errors.emplace_back("Atributo '" + node.varName + "' no existe en tipo '" + self->type + "'", node.line());
        node._type = "Error";
        return;
    }

    node._type = it->second.type;
}


Symbol* SemanticAnalyzer::lookupMethodInHierarchy(const std::string& typeName, const std::string& methodName) {
    std::cout << "[DEBUG] Buscando método '" << methodName << "' en jerarquía de tipo '" << typeName << "'\n";

    TypeSymbol* typeSym = symbolTable.lookupType(typeName);
    while (typeSym) {
        std::cout << "  [DEBUG] Buscando en tipo: " << typeSym->name << "\n";
        auto it = typeSym->methods.find(methodName);
        if (it != typeSym->methods.end()) {
            std::cout << "    [ENCONTRADO] Método '" << methodName << "' en tipo '" << typeSym->name << "'\n";
            return &it->second;
        }
        if (typeSym->parentType.empty()) break;
        typeSym = symbolTable.lookupType(typeSym->parentType);
    }

    std::cout << "    [NO ENCONTRADO] Método '" << methodName << "' no existe en jerarquía\n";
    return nullptr;
}



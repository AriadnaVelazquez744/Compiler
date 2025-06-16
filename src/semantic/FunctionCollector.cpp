//FunctionCollector.cpp
#include "FunctionCollector.hpp"
#include "../ast/AST.hpp"

FunctionCollector::FunctionCollector(SymbolTable& symTable, std::vector<SemanticError>& err)
    : symbolTable(symTable), errors(err) {}


void FunctionCollector::addBuiltins() {
    // Las funciones builtin están disponibles globalmente.
    symbolTable.addFunction("sin", "Number", {"Number"});
    symbolTable.addFunction("cos", "Number", {"Number"});
    symbolTable.addFunction("exp", "Number", {"Number"});
    symbolTable.addFunction("sqrt", "Number", {"Number"});
    symbolTable.addFunction("log", "Number", {"Number", "Number"});
    symbolTable.addFunction("min", "Number", {"Number", "Number"});
    symbolTable.addFunction("max", "Number", {"Number", "Number"});
    symbolTable.addFunction("rand", "Number", {});
    symbolTable.addFunction("print", "Null", {"Object"});
}


void FunctionCollector::visit(FunctionDeclarationNode& node) {
    if (symbolTable.existsInCurrentScope(node.name)) {
        errors.emplace_back("Función '" + node.name + "' ya declarada", node.line());
    } else {
        std::vector<std::string> paramTypes;
        for (const auto& param : *node.params) {
            paramTypes.push_back(param.type);
        }

        symbolTable.addFunction(node.name, node.returnType, paramTypes, node.body);
    }
}
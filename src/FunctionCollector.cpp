#include "FunctionCollector.hpp"
#include "AST.hpp"

FunctionCollector::FunctionCollector(SymbolTable& symTable, std::vector<SemanticError>& err)
    : symbolTable(symTable), errors(err) {}

void FunctionCollector::visit(FunctionDeclarationNode& node) {
    if (symbolTable.existsInCurrentScope(node.name)) {
        errors.emplace_back("Función '" + node.name + "' ya declarada", node.line());
    } else {
        std::vector<std::string> paramTypes;
        for (const auto& param : node.params) {
            paramTypes.push_back(param.type);
        }
        symbolTable.addFunction(node.name, node.returnType, paramTypes);
    }
}
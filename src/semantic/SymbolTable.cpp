//SymbolTable.cpp
#include "SymbolTable.hpp"

SymbolTable::SymbolTable() {
    enterScope(); // Scope global
    // Registrar constantes matemáticas
    addSymbol("PI", "Number", true);
    addSymbol("E", "Number", true);
    addType("Object", "");
    addType("Number", "Object");
    addType("String", "Object");
    addType("Boolean", "Object");
}

void SymbolTable::enterScope() {
    scopes.push_back({});
}


void SymbolTable::exitScope() {
    if (!scopes.empty()) scopes.pop_back();
}

bool SymbolTable::addSymbol(const std::string& name, const std::string& type, bool is_const, const std::vector<std::string>& params) {
    if (scopes.empty()) return false;
    auto& current = scopes.back();
    if (current.find(name) != current.end()) return false;
    current[name] = Symbol{"variable", type, is_const, params};
    return true;
}

Symbol* SymbolTable::lookup(const std::string& name) {
    // Buscar en ámbitos locales
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto entry = it->find(name);
        if (entry != it->end()) return &entry->second;
    }
    return nullptr;
}


bool SymbolTable::existsInCurrentScope(const std::string& name) {
    if (scopes.empty()) return false;
    return scopes.back().find(name) != scopes.back().end();
}


bool SymbolTable::addFunction(
    const std::string& name,
    const std::string& returnType,
    const std::vector<std::string>& params,
    ASTNode* body
) {
    if (scopes.empty()) return false;
    auto& current = scopes.back();
    if (current.find(name) != current.end()) return false;

    current[name] = Symbol{"function", returnType, false, params, body}; // incluye cuerpo
    return true;
}

bool SymbolTable::addType(
    const std::string& name,
    const std::string& parentType,
    const std::vector<std::string>& typeParams
) {
    if (types.find(name) != types.end()) return false;
    types[name] = TypeSymbol{
        name,                  // std::string name
        parentType,            // std::string parentType
        typeParams,            // std::vector<std::string> typeParams
        {},                    // Atributos (inicialmente vacío)
        {}                     // Métodos (inicialmente vacío)
    };
    return true;
}

TypeSymbol* SymbolTable::lookupType(const std::string& name) {
    auto it = types.find(name);
    if (it == types.end()) return nullptr;
    return &it->second;
}

bool SymbolTable::addTypeAttribute(const std::string& typeName, const std::string& attrName, const std::string& attrType) {
    TypeSymbol* type = lookupType(typeName);
    if (!type) return false;
    if (type->attributes.find(attrName) != type->attributes.end()) return false;
    type->attributes[attrName] = Symbol{"attribute", attrType, true, {}};
    return true;
}

bool SymbolTable::addTypeMethod(
    const std::string& typeName,
    const std::string& methodName,
    const std::string& returnType,
    const std::vector<std::string>& params
) {
    TypeSymbol* type = lookupType(typeName);
    if (!type) return false;
    if (type->methods.find(methodName) != type->methods.end()) return false;
    type->methods[methodName] = Symbol{"method", returnType, false, params};
    return true;
}

std::vector<Symbol> SymbolTable::getUserDefinedFunctions() const {
    std::vector<Symbol> functions;
    if (scopes.empty()) return functions;

    const auto& global = scopes.front();
    for (const auto& [name, sym] : global) {
        if (sym.body != nullptr) {
            functions.push_back(sym);
        }
    }
    return functions;
}
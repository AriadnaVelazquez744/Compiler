#include "SymbolTable.hpp"

SymbolTable::SymbolTable() {
    enterScope(); // Scope global
    // Registrar constantes matemáticas
    addSymbol("PI", "number", true);
    addSymbol("E", "number", true);
}

void SymbolTable::enterScope() {
    scopes.push_back({});
}

void SymbolTable::exitScope() {
    if (!scopes.empty()) scopes.pop_back();
}

bool SymbolTable::addSymbol(const std::string& name, const std::string& type, bool is_const) {
    if (scopes.empty()) return false;
    auto& current = scopes.back();
    if (current.find(name) != current.end()) return false;
    current[name] = Symbol{"variable", type, is_const, {}}; // Variables son mutables por defecto
    return true;
}

Symbol* SymbolTable::lookup(const std::string& name) {
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

bool SymbolTable::addFunction(const std::string& name, const std::string& returnType, const std::vector<std::string>& params) {
    if (scopes.empty()) return false;
    auto& current = scopes.back();
    if (current.find(name) != current.end()) return false;
    current[name] = Symbol{"function", returnType, false, params};
    return true;
}
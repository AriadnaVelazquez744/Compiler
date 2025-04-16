#pragma once
#include <vector>
#include <unordered_map>
#include <string>

struct Symbol {
    std::string type;
    bool is_const;
};

class SymbolTable {
private:
    std::vector<std::unordered_map<std::string, Symbol>> scopes;

public:
    SymbolTable();
    void enterScope();
    void exitScope();
    bool addSymbol(const std::string& name, const std::string& type, bool is_const);
    Symbol* lookup(const std::string& name);
    bool existsInCurrentScope(const std::string& name);
};
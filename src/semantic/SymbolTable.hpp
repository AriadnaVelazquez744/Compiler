//SymbolTable.hpp
#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <set> 
#include "../ast/AST.hpp"


// Tipos básicos de HULK (predefinidos)
const std::set<std::string> BUILTIN_TYPES = {"Object", "Number", "String", "Boolean", "Null"};

// Símbolo genérico (variables, funciones, tipos)
struct Symbol {
    std::string kind;       // "variable", "function", "type"
    std::string type;       // Tipo del símbolo (para variables) o tipo de retorno (funciones)
    bool is_const;          // ¿Es constante? 
    std::vector<std::string> params; // Parámetros (funciones) o tipoParams (tipos)
    ASTNode* body = nullptr;

    // Constructor completo
    Symbol(std::string kind, std::string type, bool is_const, std::vector<std::string> params, ASTNode* body = nullptr)
        : kind(std::move(kind)), type(std::move(type)), is_const(is_const), params(std::move(params)), body(body) {}

    // Constructor mínimo 
    Symbol(std::string type, bool is_const, std::vector<std::string> params = {}, ASTNode* body = nullptr)
        : kind("variable"), type(std::move(type)), is_const(is_const), params(std::move(params)), body(body) {}

    Symbol() = default;
};

// Símbolo específico para tipos (hereda de Symbol)
struct TypeSymbol {
    std::string name; 
    std::string parentType;                 // Tipo padre (si hereda)
    std::vector<std::string> typeParams;    // Parámetros genéricos 
    std::unordered_map<std::string, Symbol> attributes; // Atributos
    std::unordered_map<std::string, Symbol> methods;    // Métodos
};

class SymbolTable {
private:
    // Ámbitos para variables/funciones 
    std::vector<std::unordered_map<std::string, Symbol>> scopes;
    
    // Tabla global de tipos 
    std::unordered_map<std::string, TypeSymbol> types;


public:
    SymbolTable();
    
    // Métodos para variables/funciones
    void enterScope();
    void exitScope();
    bool addSymbol(const std::string& name, const std::string& type, bool is_const, const std::vector<std::string>& params = {});
    Symbol* lookup(const std::string& name);
    bool existsInCurrentScope(const std::string& name);
    std::string lowestCommonAncestor(const std::vector<std::string>& types) ;
    bool isSubtype(const std::string& subtype, const std::string& supertype) ;

    bool addFunction(
    const std::string& name,
    const std::string& returnType,
    const std::vector<std::string>& params,
    ASTNode* body = nullptr 
);

    // Métodos para tipos
    bool addType(
        const std::string& name,
        const std::string& parentType,
        const std::vector<std::string>& typeParams = {}
    );
    const TypeSymbol* lookupType(const std::string& name) const;
    TypeSymbol* lookupType(const std::string& name);
    
    // Métodos para atributos y métodos de tipos
    bool addTypeAttribute(const std::string& typeName, const std::string& attrName, const std::string& attrType);
    bool addTypeMethod(
        const std::string& typeName,
        const std::string& methodName,
        const std::string& returnType,
        const std::vector<std::string>& params
    );

    std::vector<Symbol> getUserDefinedFunctions() const;
    bool updateSymbolType(const std::string& name, const std::string& newType);
    void updateTypeParams(const std::string& typeName, const std::vector<std::string>& params);


};
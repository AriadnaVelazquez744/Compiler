#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <set> 


// Tipos básicos de HULK (predefinidos)
const std::set<std::string> BUILTIN_TYPES = {"Object", "Number", "String", "Boolean"};

// Símbolo genérico (variables, funciones, tipos)
struct Symbol {
    std::string kind;       // "variable", "function", "type"
    std::string type;       // Tipo del símbolo (para variables) o tipo de retorno (funciones)
    bool is_const;          // ¿Es constante? (solo aplica a variables)
    std::vector<std::string> params; // Parámetros (funciones) o tipoParams (tipos)
};

// Símbolo específico para tipos (hereda de Symbol)
struct TypeSymbol {
    std::string name; 
    std::string parentType;                 // Tipo padre (si hereda)
    std::vector<std::string> typeParams;    // Parámetros genéricos (ej: Point<T>)
    std::unordered_map<std::string, Symbol> attributes; // Atributos (nombre: tipo)
    std::unordered_map<std::string, Symbol> methods;    // Métodos (nombre: firma)
};

class SymbolTable {
private:
    // Ámbitos para variables/funciones (pila de tablas)
    std::vector<std::unordered_map<std::string, Symbol>> scopes;
    
    // Tabla global de tipos (nombre: TypeSymbol)
    std::unordered_map<std::string, TypeSymbol> types;

public:
    SymbolTable();
    
    // Métodos para variables/funciones
    void enterScope();
    void exitScope();
    bool addSymbol(const std::string& name, const std::string& type, bool is_const, const std::vector<std::string>& params = {});
    Symbol* lookup(const std::string& name);
    bool existsInCurrentScope(const std::string& name);
    bool addFunction(const std::string& name, const std::string& returnType, const std::vector<std::string>& params);

    // Métodos para tipos
    bool addType(
        const std::string& name,
        const std::string& parentType,
        const std::vector<std::string>& typeParams = {}
    );
    TypeSymbol* lookupType(const std::string& name);
    
    // Métodos para atributos y métodos de tipos
    bool addTypeAttribute(const std::string& typeName, const std::string& attrName, const std::string& attrType);
    bool addTypeMethod(
        const std::string& typeName,
        const std::string& methodName,
        const std::string& returnType,
        const std::vector<std::string>& params
    );
};
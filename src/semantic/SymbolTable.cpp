//SymbolTable.cpp
#include "SymbolTable.hpp"

SymbolTable::SymbolTable() {
    enterScope(); // Scope global
    // Registrar constantes matemáticas
    addSymbol("pi", "Number", true);
    addSymbol("e", "Number", true);
    addType("Object", "");
    addType("Number", "Object");
    addType("String", "Object");
    addType("Boolean", "Object");
    addType("Null", "");
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

    current[name] = Symbol{"function", returnType, false, params, body}; 
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
        {},                    // Atributos 
        {}                     // Métodos 
    };
    return true;
}

TypeSymbol* SymbolTable::lookupType(const std::string& name) {
    auto it = types.find(name);
    if (it == types.end()) return nullptr;
    return &it->second;
}

const TypeSymbol* SymbolTable::lookupType(const std::string& name) const {
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

bool SymbolTable::updateSymbolType(const std::string& name, const std::string& newType) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto symIt = it->find(name);
        if (symIt != it->end()) {
            symIt->second.type = newType;
            return true;
        }
    }
    return false;
}

// Determina si subtype es igual o hereda de supertype
bool SymbolTable::isSubtype(const std::string& subtype, const std::string& supertype) {
    if (subtype == supertype) return true;

    TypeSymbol* type = lookupType(subtype);
    while (type && !type->parentType.empty()) {
        if (type->parentType == supertype) return true;
        type = lookupType(type->parentType);
    }
    return false;
}

// Encuentra el ancestro común más bajo (más específico) de una lista de tipos
std::string SymbolTable::lowestCommonAncestor(const std::vector<std::string>& types) {
    if (types.empty()) return "Object";
    if (types.size() == 1) return types[0];

    // Obtener la cadena de ancestros para el primer tipo
    std::vector<std::string> ancestors1;
    std::string current = types[0];
    ancestors1.push_back(current);
    
    TypeSymbol* type = lookupType(current);
    while (type && !type->parentType.empty()) {
        ancestors1.push_back(type->parentType);
        type = lookupType(type->parentType);
    }

    // Para cada tipo restante, encontrar el ancestro común más bajo
    std::string result = types[0];
    for (size_t i = 1; i < types.size(); ++i) {
        std::string other = types[i];
        
        // Si uno es subtipo del otro, usar el supertipo
        if (isSubtype(result, other)) {
            result = other;
            continue;
        }
        if (isSubtype(other, result)) {
            continue;
        }

        // Obtener la cadena de ancestros para el otro tipo
        std::vector<std::string> ancestors2;
        current = other;
        ancestors2.push_back(current);
        
        type = lookupType(current);
        while (type && !type->parentType.empty()) {
            ancestors2.push_back(type->parentType);
            type = lookupType(type->parentType);
        }

        // Encontrar el primer ancestro común
        bool found = false;
        for (const auto& anc1 : ancestors1) {
            for (const auto& anc2 : ancestors2) {
                if (anc1 == anc2) {
                    result = anc1;
                    found = true;
                    break;
                }
            }
            if (found) break;
        }

        if (!found) {
            result = "Object";
        }

        // Actualizar ancestors1 para la siguiente iteración
        ancestors1.clear();
        current = result;
        ancestors1.push_back(current);
        type = lookupType(current);
        while (type && !type->parentType.empty()) {
            ancestors1.push_back(type->parentType);
            type = lookupType(type->parentType);
        }
    }

    return result;
}

void SymbolTable::updateTypeParams(const std::string& typeName, const std::vector<std::string>& params) {
    auto it = types.find(typeName);
    if (it != types.end()) {
        it->second.typeParams = params;
    }
}

// ========================================
// Implementación de métodos para protocolos
// ========================================

bool SymbolTable::addProtocol(const std::string& name, const std::string& baseProtocol) {
    if (protocols.find(name) != protocols.end()) return false;
    
    protocols[name] = ProtocolSymbol{
        name,                   // std::string name
        baseProtocol,           // std::string baseProtocol
        {},                     // std::unordered_map<std::string, Symbol> methods
        {}                      // std::set<std::string> extendedProtocols
    };
    
    // Si tiene protocolo base, agregarlo a las extensiones
    if (!baseProtocol.empty()) {
        protocols[name].extendedProtocols.insert(baseProtocol);
    }
    
    return true;
}

const ProtocolSymbol* SymbolTable::lookupProtocol(const std::string& name) const {
    auto it = protocols.find(name);
    if (it == protocols.end()) return nullptr;
    return &it->second;
}

ProtocolSymbol* SymbolTable::lookupProtocol(const std::string& name) {
    auto it = protocols.find(name);
    if (it == protocols.end()) return nullptr;
    return &it->second;
}

bool SymbolTable::addProtocolMethod(const std::string& protocolName, const std::string& methodName, 
                                   const std::string& returnType, const std::vector<std::string>& params) {
    ProtocolSymbol* protocol = lookupProtocol(protocolName);
    if (!protocol) return false;
    
    if (protocol->methods.find(methodName) != protocol->methods.end()) return false;
    
    protocol->methods[methodName] = Symbol{"method", returnType, false, params};
    return true;
}

bool SymbolTable::checkMethodSignatureCompatibility(const Symbol& typeMethod, const Symbol& protocolMethod) {
    // Verificar que tienen el mismo número de parámetros
    if (typeMethod.params.size() != protocolMethod.params.size()) return false;
    
    // Verificar que los tipos de parámetros son compatibles (contravariantes)
    for (size_t i = 0; i < typeMethod.params.size(); ++i) {
        // Para parámetros: tipo del protocolo debe conformar al tipo de la implementación
        // (contravariante)
        if (!isSubtype(protocolMethod.params[i], typeMethod.params[i])) {
            return false;
        }
    }
    
    // Verificar que el tipo de retorno es compatible (covariante)
    // Para retorno: tipo de la implementación debe conformar al tipo del protocolo
    if (!isSubtype(typeMethod.type, protocolMethod.type)) {
        return false;
    }
    
    return true;
}

bool SymbolTable::conformsToProtocol(const std::string& typeName, const std::string& protocolName) {
    // Verificar que el protocolo existe
    const ProtocolSymbol* protocol = lookupProtocol(protocolName);
    if (!protocol) return false;
    
    // Verificar que el tipo existe
    const TypeSymbol* type = lookupType(typeName);
    if (!type) return false;
    
    // Verificar que el tipo tiene todos los métodos requeridos por el protocolo
    for (const auto& [methodName, protocolMethod] : protocol->methods) {
        auto it = type->methods.find(methodName);
        if (it == type->methods.end()) {
            return false; // Método no encontrado
        }
        
        // Verificar compatibilidad de firma
        if (!checkMethodSignatureCompatibility(it->second, protocolMethod)) {
            return false;
        }
    }
    
    return true;
}

std::set<std::string> SymbolTable::getConformingProtocols(const std::string& typeName) {
    std::set<std::string> conformingProtocols;
    
    for (const auto& [protocolName, protocol] : protocols) {
        if (conformsToProtocol(typeName, protocolName)) {
            conformingProtocols.insert(protocolName);
        }
    }
    
    return conformingProtocols;
}

void SymbolTable::computeProtocolExtensions() {
    // Computar extensiones transitivas de protocolos
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (auto& [protocolName, protocol] : protocols) {
            // Agregar extensiones del protocolo base
            if (!protocol.baseProtocol.empty()) {
                const ProtocolSymbol* baseProtocol = lookupProtocol(protocol.baseProtocol);
                if (baseProtocol) {
                    for (const auto& extendedProtocol : baseProtocol->extendedProtocols) {
                        if (protocol.extendedProtocols.insert(extendedProtocol).second) {
                            changed = true;
                        }
                    }
                }
            }
        }
    }
}



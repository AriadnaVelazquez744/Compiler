#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include "../ast/AST.hpp"

/**
 * @brief Represents a type's method with its parameters and body
 */
struct TypeMethod {
    std::vector<Parameter>* params;
    ASTNode* body;
    std::string returnType;

    TypeMethod(std::vector<Parameter>* p, ASTNode* b, std::string ret = "")
        : params(p), body(b), returnType(std::move(ret)) {}
};

/**
 * @brief Represents a type's attribute with its name, type and initialization
 */
struct TypeAttribute {
    std::string AttrName;           // Attribute name
    std::string TypeName;          // Attribute type (Number, String, Boolean, etc.)
    ASTNode* initializer;      // Initializer AST node

    TypeAttribute(std::string attrName, std::string attrType, ASTNode* init)
        : AttrName(std::move(attrName))
        , TypeName(std::move(attrType))
        , initializer(init) {}
};

/**
 * @brief Represents a type definition with its attributes, methods and inheritance
 */
struct TypeDefinition {
    std::string name;
    std::optional<std::string> parentType;
    std::map<std::string, TypeAttribute> attributes;
    std::map<std::string, TypeMethod> methods;

    TypeDefinition(std::string n, std::optional<std::string> parent = std::nullopt)
        : name(std::move(n)), parentType(std::move(parent)) {}
};

/**
 * @brief Manages the type system including type definitions and instances
 */
class TypeSystem {
private:
    // Maps type names to their definitions
    std::map<std::string, TypeDefinition> typeTable;
    
    // Maps instance variable names to their type names
    std::map<std::string, std::string> instanceTable;
    
    // Current type being processed (for self and base calls)
    std::string currentType;

public:
    /**
     * @brief Registers a new type definition
     * @param name Type name
     * @param parent Optional parent type name
     * @return Reference to the created type definition
     */
    TypeDefinition& registerType(const std::string& name, std::optional<std::string> parent = std::nullopt);

    /**
     * @brief Adds an attribute to a type
     * @param attrName Attribute name
     * @param typeName Type name
     * @param initializer Attribute initializer AST node
     */
    void addAttribute(const std::string& attrName, const std::string& typeName, ASTNode* initializer);

    /**
     * @brief Adds a method to a type
     * @param typeName Type name
     * @param methodName Method name
     * @param params Method parameters
     * @param body Method body AST node
     * @param returnType Method return type
     */
    void addMethod(const std::string& typeName, const std::string& methodName,
                  std::vector<Parameter>* params, ASTNode* body, const std::string& returnType = "");

    /**
     * @brief Creates a new instance of a type
     * @param varName Variable name for the instance
     * @param typeName Type name
     */
    void createInstance(const std::string& varName, const std::string& typeName);

    /**
     * @brief Gets the type name for an instance variable
     * @param varName Variable name
     * @return Type name if found, empty string otherwise
     */
    std::string getInstanceType(const std::string& varName) const;

    /**
     * @brief Sets the current type being processed
     * @param typeName Type name
     */
    void setCurrentType(const std::string& typeName) { currentType = typeName; }

    /**
     * @brief Gets the current type being processed
     * @return Current type name
     */
    std::string getCurrentType() const { return currentType; }

    /**
     * @brief Finds a method in a type or its parent types
     * @param typeName Type name
     * @param methodName Method name
     * @return Pointer to the method if found, nullptr otherwise
     */
    TypeMethod* findMethod(const std::string& typeName, const std::string& methodName);

    /**
     * @brief Finds an attribute in a type or its parent types
     * @param typeName Type name
     * @param attrName Attribute name
     * @return Pointer to the attribute if found, nullptr otherwise
     */
    TypeAttribute* findAttribute(const std::string& typeName, const std::string& attrName);

    /**
     * @brief Checks if a type exists
     * @param typeName Type name
     * @return true if type exists, false otherwise
     */
    bool typeExists(const std::string& typeName) const {
        return typeTable.find(typeName) != typeTable.end();
    }
}; 
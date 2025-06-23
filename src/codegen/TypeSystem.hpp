#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include "../ast/AST.hpp"
#include <llvm/IR/Value.h>

/**
 * @brief Represents a placeholder entry with name and type
 */
struct PlaceholderEntry {
    std::string name;
    std::string type;  // "var" or "method"

    PlaceholderEntry(std::string n, std::string t) : name(std::move(n)), type(std::move(t)) {}
};

/**
 * @brief Represents a type's method with its parameters and body
 */
struct TypeMethod {
    std::vector<std::shared_ptr<Parameter>> params;
    std::shared_ptr<ASTNode> body;
    std::string returnType;

    TypeMethod(std::vector<std::shared_ptr<Parameter>> p, std::shared_ptr<ASTNode> b, std::string ret = "")
        : params(std::move(p)), body(std::move(b)), returnType(std::move(ret)) {}
};

/**
 * @brief Represents a type's attribute with its name, type and initialization
 */
struct TypeAttribute {
    std::string AttrName;           // Attribute name
    std::string TypeName;          // Attribute type (Number, String, Boolean, etc.)
    std::shared_ptr<ASTNode> initializer;      // Initializer AST node

    TypeAttribute(std::string attrName, std::string attrType, std::shared_ptr<ASTNode> init)
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
    std::vector<std::string> constructorParams;  // List of parameter names
    std::vector<std::shared_ptr<ASTNode>> baseArgs;            // Arguments for base constructor

    TypeDefinition(std::string n, std::optional<std::string> parent = std::nullopt)
        : name(std::move(n)), parentType(std::move(parent)), 
          constructorParams(), 
          baseArgs() {}
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
    
    // Maps instance names to their attribute values with type information
    std::map<std::string, std::map<std::pair<std::string, std::string>, llvm::Value*>> instanceVars;
    
    // Current type being processed (for self and base calls)
    std::string currentType;

    // Stack to track variables being processed with their types
    std::vector<PlaceholderEntry> placeholderStack;

    // Stack to track current instance variables with type information
    std::vector<std::map<std::pair<std::string, std::string>, llvm::Value*>> currentInstanceVarsStack;

public:
    /**
     * @brief Registers a new type definition
     * @param name Type name
     * @param parent Optional parent type name
     * @return Reference to the created type definition
     */
    TypeDefinition& registerType(const std::string& name, std::optional<std::string> parent = std::nullopt);

    /**
     * @brief Gets a type definition by name
     * @param typeName Type name
     * @return Reference to the type definition
     */
    TypeDefinition& getTypeDefinition(const std::string& typeName) {
        auto it = typeTable.find(typeName);
        if (it == typeTable.end()) {
            throw std::runtime_error("Type '" + typeName + "' not found");
        }
        return it->second;
    }

    /**
     * @brief Gets the constructor parameters for a type
     * @param typeName Type name
     * @return Vector of constructor parameter names
     */
    const std::vector<std::string>& getConstructorParams(const std::string& typeName) {
        return getTypeDefinition(typeName).constructorParams;
    }

    /**
     * @brief Gets the base arguments for a type
     * @param typeName Type name
     * @return Vector of base argument AST nodes
     */
    const std::vector<std::shared_ptr<ASTNode>>& getBaseArgs(const std::string& typeName) {
        return getTypeDefinition(typeName).baseArgs;
    }

    /**
     * @brief Gets the attributes for a type
     * @param typeName Type name
     * @return Map of attribute names to their definitions
     */
    const std::map<std::string, TypeAttribute>& getAttributes(const std::string& typeName) {
        return getTypeDefinition(typeName).attributes;
    }

    /**
     * @brief Gets the parent type name for a type
     * @param typeName Type name
     * @return Optional parent type name
     */
    std::optional<std::string> getParentType(const std::string& typeName) {
        return getTypeDefinition(typeName).parentType;
    }

    /**
     * @brief Adds an attribute to a type
     * @param attrName Attribute name
     * @param typeName Type name
     * @param initializer Attribute initializer AST node
     */
    void addAttribute(const std::string& attrName, const std::string& typeName, std::shared_ptr<ASTNode> initializer);

    /**
     * @brief Adds a method to a type
     * @param typeName Type name
     * @param methodName Method name
     * @param params Method parameters
     * @param body Method body AST node
     * @param returnType Method return type
     */
    void addMethod(const std::string& typeName, const std::string& methodName,
                  std::vector<std::shared_ptr<Parameter>> params, std::shared_ptr<ASTNode> body, const std::string& returnType = "");

    /**
     * @brief Creates a new instance of a type
     * @param varName Variable name for the instance
     * @param typeName Type name
     * @param vars Map of attribute names and their types to their values
     */
    void createInstance(const std::string& varName, const std::string& typeName, 
                       const std::map<std::pair<std::string, std::string>, llvm::Value*>& vars = {});

    /**
     * @brief Gets the type name for an instance variable
     * @param varName Variable name
     * @return Type name if found, empty string otherwise
     */
    std::string getInstanceType(const std::string& varName) const;

    /**
     * @brief Gets the attribute values for an instance
     * @param instanceName Instance name
     * @return Map of attribute names and types to their values
     */
    const std::map<std::pair<std::string, std::string>, llvm::Value*>& getInstanceVars(const std::string& instanceName) const {
        auto it = instanceVars.find(instanceName);
        if (it == instanceVars.end()) {
            throw std::runtime_error("Instance '" + instanceName + "' not found");
        }
        return it->second;
    }

    /**
     * @brief Sets an attribute value for an instance
     * @param instanceName Instance name
     * @param attrName Attribute name
     * @param attrType Attribute type
     * @param value Attribute value
     */
    void setInstanceVar(const std::string& instanceName, const std::string& attrName, 
                       const std::string& attrType, llvm::Value* value) {
        instanceVars[instanceName][{attrName, attrType}] = value;
    }

    /**
     * @brief Gets an attribute value for an instance
     * @param instanceName Instance name
     * @param attrName Attribute name
     * @param attrType Attribute type
     * @return Attribute value if found, nullptr otherwise
     */
    llvm::Value* getInstanceVar(const std::string& instanceName, const std::string& attrName, 
                               const std::string& attrType) const {
        auto it = instanceVars.find(instanceName);
        if (it == instanceVars.end()) return nullptr;
        
        auto attrIt = it->second.find({attrName, attrType});
        return attrIt != it->second.end() ? attrIt->second : nullptr;
    }

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

    /**
     * @brief Pushes a variable name and type onto the placeholder stack
     * @param varName Variable name to push
     * @param varType Variable type to push
     */
    void pushPlaceholder(const std::string& varName, const std::string& varType) {
        placeholderStack.push_back(PlaceholderEntry(varName, varType));
    }

    /**
     * @brief Pops a variable name and type from the placeholder stack
     * @return The popped PlaceholderEntry
     */
    PlaceholderEntry popPlaceholder() {
        if (placeholderStack.empty()) {
            return PlaceholderEntry("", "");
        }
        PlaceholderEntry entry = placeholderStack.back();
        placeholderStack.pop_back();
        return entry;
    }

    /**
     * @brief Gets the current variable name and type being processed
     * @return Current PlaceholderEntry, or empty entry if stack is empty
     */
    PlaceholderEntry getCurrentPlaceholder() const {
        return placeholderStack.empty() ? PlaceholderEntry("", "") : placeholderStack.back();
    }

    /**
     * @brief Checks if the placeholder stack is empty
     * @return true if stack is empty, false otherwise
     */
    bool isPlaceholderStackEmpty() const {
        return placeholderStack.empty();
    }

    /**
     * @brief Pushes a new instance variables map onto the stack
     * @param vars Map of instance variables to push
     */
    void pushCurrentInstanceVars(const std::map<std::pair<std::string, std::string>, llvm::Value*>& vars) {
        currentInstanceVarsStack.push_back(vars);
    }

    /**
     * @brief Pops the top instance variables map from the stack
     */
    void popCurrentInstanceVars() {
        if (!currentInstanceVarsStack.empty()) {
            currentInstanceVarsStack.pop_back();
        }
    }

    /**
     * @brief Sets a value in the current instance variables map
     * @param varName Variable name to set
     * @param varType Variable type to set
     * @param value Value to set
     */
    void setCurrentInstanceVar(const std::string& varName, const std::string& varType, llvm::Value* value) {
        if (!currentInstanceVarsStack.empty()) {
            currentInstanceVarsStack.back()[{varName, varType}] = value;
        }
    }

    /**
     * @brief Gets a value from the current instance variables map
     * @param varName Variable name to look up
     * @param varType Variable type to look up
     * @return Pointer to the value if found, nullptr otherwise
     */
    llvm::Value* getCurrentInstanceVar(const std::string& varName, const std::string& varType) const {
        if (currentInstanceVarsStack.empty()) {
            return nullptr;
        }
        auto& currentVars = currentInstanceVarsStack.back();
        auto it = currentVars.find({varName, varType});
        return it != currentVars.end() ? it->second : nullptr;
    }

    /**
     * @brief Gets all instance names
     * @return Vector of instance names
     */
    std::vector<std::string> getAllInstanceNames() const {
        std::vector<std::string> names;
        for (const auto& [name, _] : instanceTable) {
            names.push_back(name);
        }
        return names;
    }

    /**
     * @brief Checks if the instance variables stack is empty
     * @return true if stack is empty, false otherwise
     */
    bool isInstanceVarsStackEmpty() const {
        return currentInstanceVarsStack.empty();
    }
}; 
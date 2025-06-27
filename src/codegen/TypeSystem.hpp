#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include "../ast/AST.hpp"
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

// Forward declarations
namespace llvm {
    class Module;
    class LLVMContext;
    class StructType;
    class Function;
    class Value;
}

class LLVMGenerator; // Forward declaration

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
    std::vector<Parameter>* params;
    ASTNode* body;
    std::string returnType;
    llvm::Function* llvmFunction;  // LLVM function for this method

    TypeMethod(std::vector<Parameter>* p, ASTNode* b, std::string ret = "")
        : params(p), body(b), returnType(std::move(ret)), llvmFunction(nullptr) {}
};

/**
 * @brief Represents a type's attribute with its name, type and initialization
 */
struct TypeAttribute {
    std::string AttrName;           // Attribute name
    std::string TypeName;          // Attribute type (Number, String, Boolean, etc.)
    ASTNode* initializer;      // Initializer AST node
    llvm::Type* llvmType;      // LLVM type for this attribute

    TypeAttribute(std::string attrName, std::string attrType, ASTNode* init)
        : AttrName(std::move(attrName))
        , TypeName(std::move(attrType))
        , initializer(init)
        , llvmType(nullptr) {}
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
    std::vector<ASTNode*> baseArgs;            // Arguments for base constructor
    
    // LLVM-specific fields
    llvm::StructType* llvmStructType;          // LLVM struct type
    llvm::StructType* vtableType;              // VTable type for this class
    llvm::GlobalVariable* vtableGlobal;        // Global vtable instance
    int typeId;                                // Unique type ID for runtime type checking

    TypeDefinition(std::string n, std::optional<std::string> parent = std::nullopt)
        : name(std::move(n)), parentType(std::move(parent)), 
          constructorParams(), baseArgs(),
          llvmStructType(nullptr), vtableType(nullptr), vtableGlobal(nullptr), typeId(0) {}
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

    // LLVM context and module references
    llvm::LLVMContext* llvmContext;
    llvm::Module* llvmModule;
    
    // Type ID counter for runtime type checking
    int nextTypeId;

    // Helper methods for LLVM type generation
    void generateLLVMStructType(TypeDefinition& typeDef);
    void generateVTableType(TypeDefinition& typeDef);
    void generateVTableGlobal(TypeDefinition& typeDef);
    void generateMethodFunctions(TypeDefinition& typeDef);
    void generateMethodBody(TypeMethod& method, TypeDefinition& typeDef, llvm::IRBuilder<>& builder);

public:
    /**
     * @brief Constructor with LLVM context and module
     */
    TypeSystem(llvm::LLVMContext* context, llvm::Module* module);

    /**
     * @brief Gets the LLVM type for a Hulk type name
     * @param hulkType Type name (Number, String, Boolean, etc.)
     * @return LLVM type pointer
     */
    llvm::Type* getLLVMTypeForHulkType(const std::string& hulkType);

    /**
     * @brief Registers a new type definition and generates LLVM types
     * @param name Type name
     * @param parent Optional parent type name
     * @return Reference to the created type definition
     */
    TypeDefinition& registerType(const std::string& name, std::optional<std::string> parent = std::nullopt);

    /**
     * @brief Generates complete LLVM types for a registered type
     * @param typeName Type name to generate LLVM types for
     */
    void generateLLVMTypes(const std::string& typeName);

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
     * @brief Gets the LLVM struct type for a type
     * @param typeName Type name
     * @return LLVM struct type pointer
     */
    llvm::StructType* getLLVMStructType(const std::string& typeName) {
        return getTypeDefinition(typeName).llvmStructType;
    }

    /**
     * @brief Gets the vtable type for a type
     * @param typeName Type name
     * @return LLVM vtable struct type pointer
     */
    llvm::StructType* getVTableType(const std::string& typeName) {
        return getTypeDefinition(typeName).vtableType;
    }

    /**
     * @brief Gets the vtable global variable for a type
     * @param typeName Type name
     * @return LLVM vtable global variable pointer
     */
    llvm::GlobalVariable* getVTableGlobal(const std::string& typeName) {
        return getTypeDefinition(typeName).vtableGlobal;
    }

    /**
     * @brief Gets the type ID for a type
     * @param typeName Type name
     * @return Type ID
     */
    int getTypeId(const std::string& typeName) {
        return getTypeDefinition(typeName).typeId;
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
    const std::vector<ASTNode*>& getBaseArgs(const std::string& typeName) {
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
     * @param typeName Type being defined
     * @param attrName Attribute name
     * @param attrType Attribute type
     * @param initializer Attribute initializer AST node
     */
    void addAttribute(const std::string& typeName, const std::string& attrName, const std::string& attrType, ASTNode* initializer);

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
     * @return True if type exists, false otherwise
     */
    bool typeExists(const std::string& typeName) const {
        return typeTable.find(typeName) != typeTable.end();
    }

    /**
     * @brief Pushes a placeholder entry onto the stack
     * @param varName Variable name
     * @param varType Variable type
     */
    void pushPlaceholder(const std::string& varName, const std::string& varType) {
        placeholderStack.emplace_back(varName, varType);
    }

    /**
     * @brief Pops a placeholder entry from the stack
     * @return Placeholder entry
     */
    PlaceholderEntry popPlaceholder() {
        if (placeholderStack.empty()) {
            throw std::runtime_error("Placeholder stack is empty");
        }
        PlaceholderEntry entry = placeholderStack.back();
        placeholderStack.pop_back();
        return entry;
    }

    /**
     * @brief Gets the current placeholder entry
     * @return Current placeholder entry
     */
    PlaceholderEntry getCurrentPlaceholder() const {
        if (placeholderStack.empty()) {
            return PlaceholderEntry("", "");
        }
        return placeholderStack.back();
    }

    /**
     * @brief Checks if placeholder stack is empty
     * @return True if empty, false otherwise
     */
    bool isPlaceholderStackEmpty() const {
        return placeholderStack.empty();
    }

    /**
     * @brief Pushes current instance variables onto the stack
     * @param vars Instance variables map
     */
    void pushCurrentInstanceVars(const std::map<std::pair<std::string, std::string>, llvm::Value*>& vars) {
        currentInstanceVarsStack.push_back(vars);
    }

    /**
     * @brief Pops current instance variables from the stack
     */
    void popCurrentInstanceVars() {
        if (currentInstanceVarsStack.empty()) {
            throw std::runtime_error("Current instance vars stack is empty");
        }
        currentInstanceVarsStack.pop_back();
    }

    /**
     * @brief Sets a current instance variable
     * @param varName Variable name
     * @param varType Variable type
     * @param value Variable value
     */
    void setCurrentInstanceVar(const std::string& varName, const std::string& varType, llvm::Value* value) {
        if (currentInstanceVarsStack.empty()) {
            throw std::runtime_error("No current instance vars context");
        }
        currentInstanceVarsStack.back()[{varName, varType}] = value;
    }

    /**
     * @brief Gets a current instance variable
     * @param varName Variable name
     * @param varType Variable type
     * @return Variable value if found, nullptr otherwise
     */
    llvm::Value* getCurrentInstanceVar(const std::string& varName, const std::string& varType) const {
        if (currentInstanceVarsStack.empty()) {
            return nullptr;
        }
        
        const auto& currentVars = currentInstanceVarsStack.back();
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
     * @brief Checks if instance vars stack is empty
     * @return True if empty, false otherwise
     */
    bool isInstanceVarsStackEmpty() const {
        return currentInstanceVarsStack.empty();
    }

    /**
     * @brief Sets the LLVM generator for method body generation
     * @param generator Pointer to the LLVM generator
     */
    void setLLVMGenerator(LLVMGenerator* generator);

private:
    LLVMGenerator* llvmGenerator; // Reference to LLVM generator for method body generation
}; 
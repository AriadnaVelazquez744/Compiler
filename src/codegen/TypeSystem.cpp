#include "TypeSystem.hpp"

TypeDefinition& TypeSystem::registerType(const std::string& name, std::optional<std::string> parent) {
    // Check if parent type exists if specified
    if (parent && !typeExists(*parent)) {
        throw std::runtime_error("Parent type '" + *parent + "' not found for type '" + name + "'");
    }

    // Create and store the type definition
    auto [it, inserted] = typeTable.try_emplace(name, name, parent);
    if (!inserted) {
        throw std::runtime_error("Type '" + name + "' already exists");
    }
    return it->second;
}

void TypeSystem::addAttribute(const std::string& attrName, const std::string& typeName, ASTNode* initializer) {
    auto it = typeTable.find(typeName);
    if (it == typeTable.end()) {
        throw std::runtime_error("Type '" + typeName + "' not found");
    }

    // Check if attribute already exists
    if (it->second.attributes.find(attrName) != it->second.attributes.end()) {
        throw std::runtime_error("Attribute '" + attrName + "' already exists in type '" + typeName + "'");
    }

    it->second.attributes.emplace(attrName, TypeAttribute(attrName, typeName, initializer));
}

void TypeSystem::addMethod(const std::string& typeName, const std::string& methodName,
                         std::vector<Parameter>* params, ASTNode* body, const std::string& returnType) {
    auto it = typeTable.find(typeName);
    if (it == typeTable.end()) {
        throw std::runtime_error("Type '" + typeName + "' not found");
    }

    // Check if method already exists
    if (it->second.methods.find(methodName) != it->second.methods.end()) {
        throw std::runtime_error("Method '" + methodName + "' already exists in type '" + typeName + "'");
    }

    it->second.methods.emplace(methodName, TypeMethod(params, body, returnType));
}

void TypeSystem::createInstance(const std::string& varName, const std::string& typeName) {
    if (!typeExists(typeName)) {
        throw std::runtime_error("Type '" + typeName + "' not found");
    }

    // Check if instance already exists
    if (instanceTable.find(varName) != instanceTable.end()) {
        throw std::runtime_error("Instance '" + varName + "' already exists");
    }

    instanceTable[varName] = typeName;
}

std::string TypeSystem::getInstanceType(const std::string& varName) const {
    auto it = instanceTable.find(varName);
    return it != instanceTable.end() ? it->second : "";
}

TypeMethod* TypeSystem::findMethod(const std::string& typeName, const std::string& methodName) {
    auto it = typeTable.find(typeName);
    if (it == typeTable.end()) return nullptr;

    // Look for method in current type
    auto methodIt = it->second.methods.find(methodName);
    if (methodIt != it->second.methods.end()) {
        return &methodIt->second;
    }

    // If not found and has parent, look in parent type
    if (it->second.parentType) {
        return findMethod(*it->second.parentType, methodName);
    }

    return nullptr;
}

TypeAttribute* TypeSystem::findAttribute(const std::string& typeName, const std::string& attrName) {
    auto it = typeTable.find(typeName);
    if (it == typeTable.end()) return nullptr;

    // Look for attribute in current type
    auto attrIt = it->second.attributes.find(attrName);
    if (attrIt != it->second.attributes.end()) {
        return &attrIt->second;
    }

    // If not found and has parent, look in parent type
    if (it->second.parentType) {
        return findAttribute(*it->second.parentType, attrName);
    }

    return nullptr;
} 
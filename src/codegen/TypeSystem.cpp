#include "TypeSystem.hpp"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/BasicBlock.h>
#include <iostream>

TypeSystem::TypeSystem(llvm::LLVMContext* context, llvm::Module* module)
    : llvmContext(context), llvmModule(module), nextTypeId(1), llvmGenerator(nullptr) {
    // Register built-in types
    registerType("Number");
    registerType("String");
    registerType("Boolean");
    registerType("Null");
}

void TypeSystem::setLLVMGenerator(LLVMGenerator* generator) {
    llvmGenerator = generator;
}

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

    // Initialize constructor parameters and base args
    it->second.constructorParams = std::vector<std::string>();
    it->second.baseArgs = std::vector<ASTNode*>();
    
    // Assign type ID
    it->second.typeId = nextTypeId++;

    return it->second;
}

void TypeSystem::generateLLVMTypes(const std::string& typeName) {
    auto& typeDef = getTypeDefinition(typeName);
    
    // Generate LLVM struct type
    generateLLVMStructType(typeDef);
    
    // Generate vtable type
    generateVTableType(typeDef);
    
    // Generate method functions
    generateMethodFunctions(typeDef);
    
    // Generate vtable global
    generateVTableGlobal(typeDef);
    
    std::cout << "✅ Generated LLVM types for " << typeName << std::endl;
}

llvm::Type* TypeSystem::getLLVMTypeForHulkType(const std::string& hulkType) {
    if (hulkType == "Number") {
        return llvm::Type::getDoubleTy(*llvmContext);
    } else if (hulkType == "String") {
        return llvm::PointerType::get(llvm::Type::getInt8Ty(*llvmContext), 0);
    } else if (hulkType == "Boolean") {
        return llvm::Type::getInt1Ty(*llvmContext);
    } else if (hulkType == "Null") {
        return llvm::PointerType::get(llvm::Type::getInt8Ty(*llvmContext), 0); // void* equivalent
    } else {
        // Check if it's a user-defined type
        if (typeExists(hulkType)) {
            auto& typeDef = getTypeDefinition(hulkType);
            if (typeDef.llvmStructType) {
                return llvm::PointerType::get(typeDef.llvmStructType, 0);
            }
        }
        // Default to void* for unknown types
        return llvm::PointerType::get(llvm::Type::getInt8Ty(*llvmContext), 0);
    }
}

void TypeSystem::generateLLVMStructType(TypeDefinition& typeDef) {
    std::vector<llvm::Type*> fieldTypes;
    
    // Add type ID as first field (32-bit integer)
    fieldTypes.push_back(llvm::Type::getInt32Ty(*llvmContext));
    
    // Add vtable pointer as second field
    std::string vtableTypeName = typeDef.name + "_vtable";
    typeDef.vtableType = llvm::StructType::create(*llvmContext, vtableTypeName);
    fieldTypes.push_back(llvm::PointerType::get(typeDef.vtableType, 0));
    
    // Add parent pointer if inheriting
    if (typeDef.parentType) {
        if (typeExists(*typeDef.parentType)) {
            auto& parentDef = getTypeDefinition(*typeDef.parentType);
            if (parentDef.llvmStructType) {
                fieldTypes.push_back(llvm::PointerType::get(parentDef.llvmStructType, 0));
            } else {
                // Create forward declaration for parent
                parentDef.llvmStructType = llvm::StructType::create(*llvmContext, *typeDef.parentType);
                fieldTypes.push_back(llvm::PointerType::get(parentDef.llvmStructType, 0));
            }
        }
    }
    
    // Add this type's attributes
    for (auto& [attrName, attr] : typeDef.attributes) {
        std::cerr << "[DEBUG] Adding attribute to struct: '" << attrName << "' of type '" << attr.TypeName << "'\n";
        attr.llvmType = getLLVMTypeForHulkType(attr.TypeName);
        fieldTypes.push_back(attr.llvmType);
    }
    
    // Create the struct type
    typeDef.llvmStructType = llvm::StructType::create(*llvmContext, fieldTypes, typeDef.name);
    
    std::cout << "  📝 Created LLVM struct type: " << typeDef.name << std::endl;
}

void TypeSystem::generateVTableType(TypeDefinition& typeDef) {
    std::vector<llvm::Type*> vtableFields;
    
    // Add function pointers for each method
    for (auto& [methodName, method] : typeDef.methods) {
        // Create function type with 'this' pointer as first argument
        std::vector<llvm::Type*> paramTypes;
        paramTypes.push_back(llvm::PointerType::get(typeDef.llvmStructType, 0)); // 'this' pointer
        
        if (method.params) {
            for (const auto& param : *method.params) {
                paramTypes.push_back(getLLVMTypeForHulkType(param.type));
            }
        }
        
        // Get return type
        llvm::Type* returnType;
        if (!method.returnType.empty()) {
            returnType = getLLVMTypeForHulkType(method.returnType);
        } else {
            returnType = llvm::Type::getVoidTy(*llvmContext);
        }
        
        llvm::FunctionType* funcType = llvm::FunctionType::get(returnType, paramTypes, false);
        vtableFields.push_back(llvm::PointerType::get(funcType, 0));
    }
    
    // Set the vtable type body
    if (!vtableFields.empty()) {
        typeDef.vtableType->setBody(vtableFields);
    }
    
    std::cout << "  📝 Created vtable type: " << typeDef.name << "_vtable with " 
              << vtableFields.size() << " methods" << std::endl;
}

void TypeSystem::generateMethodFunctions(TypeDefinition& typeDef) {
    for (auto& [methodName, method] : typeDef.methods) {
        // Create function type with 'this' pointer as first argument
        std::vector<llvm::Type*> paramTypes;
        paramTypes.push_back(llvm::PointerType::get(typeDef.llvmStructType, 0)); // 'this' pointer
        
        if (method.params) {
            for (const auto& param : *method.params) {
                paramTypes.push_back(getLLVMTypeForHulkType(param.type));
            }
        }
        
        // Get return type
        llvm::Type* returnType;
        if (!method.returnType.empty()) {
            returnType = getLLVMTypeForHulkType(method.returnType);
        } else {
            returnType = llvm::Type::getVoidTy(*llvmContext);
        }
        
        llvm::FunctionType* funcType = llvm::FunctionType::get(returnType, paramTypes, false);
        
        // Create function with mangled name to avoid conflicts
        std::string mangledName = typeDef.name + "_" + methodName;
        method.llvmFunction = llvm::Function::Create(
            funcType, 
            llvm::Function::ExternalLinkage, 
            mangledName, 
            llvmModule
        );
        
        // Generate method body
        llvm::IRBuilder<> tempBuilder(*llvmContext);
        generateMethodBody(method, typeDef, tempBuilder);
        
        std::cout << "  📝 Created method function: " << mangledName << std::endl;
    }
}

void TypeSystem::generateMethodBody(TypeMethod& method, TypeDefinition& typeDef, llvm::IRBuilder<>& builder) {
    // Create entry block
    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(*llvmContext, "entry", method.llvmFunction);
    builder.SetInsertPoint(entryBlock);
    
    // Get 'this' pointer
    llvm::Value* thisPtr = method.llvmFunction->getArg(0);
    
    // Add parameters to local scope
    if (method.params) {
        for (size_t i = 0; i < method.params->size(); ++i) {
            llvm::Value* param = method.llvmFunction->getArg(i + 1);
            llvm::AllocaInst* paramAlloca = builder.CreateAlloca(param->getType(), nullptr, (*method.params)[i].name);
            builder.CreateStore(param, paramAlloca);
        }
    }
    
    // For now, create a simple implementation that handles basic method bodies
    // This is a simplified version that would need to be enhanced for full AST support
    if (method.body) {
        // Try to handle simple return statements
        if (auto* literal = dynamic_cast<LiteralNode*>(method.body)) {
            llvm::Value* returnValue = nullptr;
            
            if (literal->type() == "Number") {
                double value = std::stod(literal->value);
                returnValue = llvm::ConstantFP::get(*llvmContext, llvm::APFloat(value));
            } else if (literal->type() == "String") {
                returnValue = builder.CreateGlobalStringPtr(literal->value);
            } else if (literal->type() == "Boolean") {
                bool value = (literal->value == "true");
                returnValue = llvm::ConstantInt::get(*llvmContext, llvm::APInt(1, value));
            }
            
            if (returnValue) {
                llvm::Type* returnType = method.llvmFunction->getReturnType();
                if (returnType->isVoidTy()) {
                    builder.CreateRetVoid();
                } else {
                    builder.CreateRet(returnValue);
                }
                return;
            }
        }
        
        // Handle self access for simple cases
        if (auto* selfCall = dynamic_cast<SelfCallNode*>(method.body)) {
            // Dynamically find the attribute index
            int attrIndex = 2; // typeId, vtable
            if (typeDef.parentType) attrIndex++; // parent pointer
            std::string attrName = selfCall->varName;
            int i = 0;
            for (const auto& [name, attr] : typeDef.attributes) {
                if (name == attrName) {
                    attrIndex += i;
                    break;
                }
                ++i;
            }
            llvm::Value* fieldPtr = builder.CreateStructGEP(typeDef.llvmStructType, thisPtr, attrIndex, "field_ptr");
            unsigned numElements = typeDef.llvmStructType->getNumElements();
            std::cerr << "[DEBUG] Struct '" << typeDef.name << "' has " << numElements << " elements. attrIndex=" << attrIndex << std::endl;
            llvm::Type* fieldType = nullptr;
            if (attrIndex < numElements) {
                fieldType = typeDef.llvmStructType->getElementType(attrIndex);
            } else {
                std::cerr << "[ERROR] attrIndex out of range!" << std::endl;
                builder.CreateRetVoid();
                return;
            }
            llvm::Value* fieldValue = builder.CreateLoad(fieldType, fieldPtr, "field_value");
            llvm::Type* returnType = method.llvmFunction->getReturnType();
            if (!returnType->isVoidTy()) {
                builder.CreateRet(fieldValue);
            } else {
                builder.CreateRetVoid();
            }
            return;
        }
    }
    
    // Fallback: create default return
    llvm::Type* returnType = method.llvmFunction->getReturnType();
    if (returnType->isVoidTy()) {
        builder.CreateRetVoid();
    } else {
        llvm::Value* defaultValue = llvm::Constant::getNullValue(returnType);
        builder.CreateRet(defaultValue);
    }
}

void TypeSystem::generateVTableGlobal(TypeDefinition& typeDef) {
    if (!typeDef.vtableType || typeDef.methods.empty()) {
        return;
    }
    
    // Create array of function pointers for vtable
    std::vector<llvm::Constant*> vtableEntries;
    for (auto& [methodName, method] : typeDef.methods) {
        if (method.llvmFunction) {
            vtableEntries.push_back(method.llvmFunction);
        }
    }
    
    if (!vtableEntries.empty()) {
        // Create vtable constant
        llvm::Constant* vtableConstant = llvm::ConstantStruct::get(typeDef.vtableType, vtableEntries);
        
        // Create global variable for vtable
        std::string vtableName = typeDef.name + "_vtable_instance";
        typeDef.vtableGlobal = new llvm::GlobalVariable(
            *llvmModule,
            typeDef.vtableType,
            true, // isConstant
            llvm::GlobalValue::PrivateLinkage,
            vtableConstant,
            vtableName
        );
        
        std::cout << "  📝 Created vtable global: " << vtableName << std::endl;
    }
}

void TypeSystem::addAttribute(const std::string& typeName, const std::string& attrName, const std::string& attrType, ASTNode* initializer) {
    auto it = typeTable.find(typeName);
    if (it == typeTable.end()) {
        throw std::runtime_error("Type '" + typeName + "' not found");
    }

    // Check if attribute already exists
    if (it->second.attributes.find(attrName) != it->second.attributes.end()) {
        throw std::runtime_error("Attribute '" + attrName + "' already exists in type '" + typeName + "'");
    }

    it->second.attributes.emplace(attrName, TypeAttribute(attrName, attrType, initializer));
    std::cerr << "[DEBUG] addAttribute: type='" << typeName << "', attr='" << attrName << "', attrType='" << attrType << "'\n";
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

void TypeSystem::createInstance(const std::string& varName, const std::string& typeName, 
                              const std::map<std::pair<std::string, std::string>, llvm::Value*>& vars) {
    if (!typeExists(typeName)) {
        throw std::runtime_error("Type '" + typeName + "' not found");
    }

    // Check if instance already exists
    if (instanceTable.find(varName) != instanceTable.end()) {
        throw std::runtime_error("Instance '" + varName + "' already exists");
    }

    instanceTable[varName] = typeName;
    instanceVars[varName] = vars;
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
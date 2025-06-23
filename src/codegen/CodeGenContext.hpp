#pragma once

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "TypeSystem.hpp"

#include <string>
#include <map>
#include <vector> // <-- Needed for value stack
#include <memory> // <-- Needed for std::shared_ptr

// Forward declarations
class ASTNode;
class FunctionDeclarationNode;

// This class wraps all LLVM state and IR generation context
class CodeGenContext {
public:
    llvm::LLVMContext context;         // Global LLVM context (types, constants, etc.)
    llvm::IRBuilder<> builder;         // Builder for creating IR instructions
    llvm::Module module;               // LLVM IR module (a single compilation unit)

    std::map<std::string, llvm::Value*> locals; // Symbol table for local vars like 'self', loop vars, etc.
    std::vector<llvm::Value*> valueStack;       // 🔁 Stack to pass values up/down during AST traversal

    TypeSystem typeSystem;             // Type system for managing types and instances

    CodeGenContext();  // Constructor initializes builder and module

    void generateCode(std::vector<std::shared_ptr<ASTNode>>& root);                  // Main entry: triggers IR generation
    void dumpIR(const std::string& filename = "hulk-low-code.ll"); // Dumps IR to .ll file

    // Scopes
    std::vector<std::map<std::string, llvm::Value*>> localScopes;
    std::vector<std::map<std::string, std::shared_ptr<FunctionDeclarationNode>>> functionScopes;

    // Var Scopes
    void pushVarScope(bool inherit = true) { 
        if (inherit && !localScopes.empty()) {
            // Create new scope with copy of parent's variables
            localScopes.push_back(localScopes.back());
        } else {
            // Create empty scope (no inheritance)
            localScopes.emplace_back();
        }
    }
    void popVarScope()  { localScopes.pop_back(); }
    void addLocal(const std::string& name, llvm::Value* val) {
        if (!localScopes.empty()) {
            // Add or update variable in current scope
            // This will shadow any variable with the same name in parent scopes
            localScopes.back()[name] = val;
        }
    }
    llvm::Value* lookupLocal(const std::string& name) const {
        // Search from innermost to outermost scope
        for (auto it = localScopes.rbegin(); it != localScopes.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) return found->second;
        }
        return nullptr;
    }

    // Func Scopes
    void pushFuncScope() { 
        if (functionScopes.empty()) {
            functionScopes.emplace_back();
        } else {
            // Copy all functions from the parent scope
            functionScopes.push_back(functionScopes.back());
        }
    }
    void popFuncScope()  { functionScopes.pop_back(); }
    void addFuncDecl(const std::string& name, std::shared_ptr<FunctionDeclarationNode> decl) {
        if (!functionScopes.empty()) functionScopes.back()[name] = decl;
    }
    std::shared_ptr<FunctionDeclarationNode> lookupFuncDecl(const std::string& name) const {
        for (auto it = functionScopes.rbegin(); it != functionScopes.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) return found->second;
        }
        return nullptr;
    }

};

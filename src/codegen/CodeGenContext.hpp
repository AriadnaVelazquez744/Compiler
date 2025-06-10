#pragma once

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"

#include <string>
#include <map>
#include <vector> // <-- Needed for value stack

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

    CodeGenContext();  // Constructor initializes builder and module

    void generateCode(std::vector<ASTNode*>& root);                  // Main entry: triggers IR generation
    void dumpIR(const std::string& filename = "hulk-low-code.ll"); // Dumps IR to .ll file

    // Scopes
    std::vector<std::map<std::string, llvm::Value*>> localScopes;
    std::vector<std::map<std::string, FunctionDeclarationNode*>> functionScopes;

    // Var Scopes
    void pushVarScope() { localScopes.emplace_back(); }
    void popVarScope()  { localScopes.pop_back(); }
    void addLocal(const std::string& name, llvm::Value* val) {
        if (!localScopes.empty()) localScopes.back()[name] = val;
    }
    llvm::Value* lookupLocal(const std::string& name) const {
        for (auto it = localScopes.rbegin(); it != localScopes.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) return found->second;
        }
        return nullptr;
    }

    // Func Scopes
    void pushFuncScope() { functionScopes.emplace_back(); }
    void popFuncScope()  { functionScopes.pop_back(); }
    void addFuncDecl(const std::string& name, FunctionDeclarationNode* decl) {
        if (!functionScopes.empty()) functionScopes.back()[name] = decl;
    }
    FunctionDeclarationNode* lookupFuncDecl(const std::string& name) const {
        for (auto it = functionScopes.rbegin(); it != functionScopes.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) return found->second;
        }
        return nullptr;
    }

};

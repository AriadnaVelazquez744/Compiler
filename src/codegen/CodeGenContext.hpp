#pragma once

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"

#include <string>
#include <map>
#include <vector> // <-- Needed for value stack

class ASTNode;

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
};

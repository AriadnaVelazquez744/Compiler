#include <cstdio>
#include <iostream>
#include "CodeGenContext.hpp"
#include "LLVMGenerator.hpp"
#include "../ast/AST.hpp"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"

CodeGenContext::CodeGenContext()
    : builder(context), module("hulk_module", context) {}  // Associate module and builder with context

void CodeGenContext::generateCode(ASTNode* root) {
    // Create the 'main' function which will be the entry point
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context), false);
    llvm::Function* mainFunc = llvm::Function::Create(
        funcType, llvm::Function::ExternalLinkage, "main", module);
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", mainFunc);
    builder.SetInsertPoint(entry); // Set current insertion point to entry

    LLVMGenerator generator(*this); // Create visitor
    root->accept(generator);        // Start visiting nodes

    builder.CreateRetVoid();       // Emit return from main
    llvm::verifyFunction(*mainFunc); // Validate generated IR
}

void CodeGenContext::dumpIR(const std::string& filename) {
    std::error_code EC;
    llvm::raw_fd_ostream out(filename, EC);

    if (EC) {
        std::cerr << "❌ Error: " << EC.message() << std::endl;
        return;
    }

    module.print(out, nullptr); // Emit IR
    out.flush();
    std::cout << "✅ IR dumped to: " << filename << std::endl;
}

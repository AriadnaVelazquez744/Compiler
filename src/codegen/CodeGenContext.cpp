#include <cstdio>
#include <iostream>
#include "../ast/AST.hpp"
#include "CodeGenContext.hpp"
#include "LLVMGenerator.hpp"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"

CodeGenContext::CodeGenContext()
    : builder(context), module("hulk_module", context) {}  // Associate module and builder with context

void CodeGenContext::generateCode(std::vector<ASTNode*>& root) {
    
    // Separate the function declaration from the expression declarations for context
    pushFuncScope();  // Global function registry

    std::vector<ASTNode*> exprs;
    for (ASTNode* node : root) {
        if (auto* fn = dynamic_cast<FunctionDeclarationNode*>(node)) {
            addFuncDecl(fn->name, fn);
        } else {
            exprs.push_back(node);
        }
    }
    root = std::move(exprs);

    // Declare printf and puts for printing
    llvm::FunctionType* printfType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(context),
        {llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0)},
        true // Variadic function
    );
    llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", &module);

    llvm::FunctionType* putsType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(context),
        {llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0)},
        false
    );
    llvm::Function::Create(putsType, llvm::Function::ExternalLinkage, "puts", &module);

    // Create main function with i32 return type
    llvm::FunctionType* mainType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(context), false
    );
    llvm::Function* mainFunc = llvm::Function::Create(
        mainType, llvm::Function::ExternalLinkage, "main", module
    );
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", mainFunc);
    builder.SetInsertPoint(entry);

    // Generar código para todos los nodos AST
    LLVMGenerator generator(*this);
    for (ASTNode* node : root) { // 🆕 Iterar sobre cada nodo
        node->accept(generator);
    }

    // Print all remaining values on the stack
    for (llvm::Value* val : valueStack) {
        if (val->getType()->isDoubleTy()) { // Number
            llvm::Value* format = builder.CreateGlobalStringPtr("%g\n");
            builder.CreateCall(module.getFunction("printf"), {format, val});
        } else if (val->getType()->isIntegerTy(1)) { // Boolean
            llvm::Value* str = builder.CreateSelect(
                val,
                builder.CreateGlobalStringPtr("true\n"),
                builder.CreateGlobalStringPtr("false\n")
            );
            builder.CreateCall(module.getFunction("puts"), {str});
        } else if (val->getType()->isPointerTy()) { // String
            builder.CreateCall(module.getFunction("puts"), {val});
        }
    }

    // Clear the stack after printing
    valueStack.clear();

    // Return 0 from main
    builder.CreateRet(llvm::ConstantInt::get(context, llvm::APInt(32, 0)));
    llvm::verifyFunction(*mainFunc);
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

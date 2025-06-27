#include <cstdio>
#include <iostream>
#include "../ast/AST.hpp"
#include "CodeGenContext.hpp"
#include "LLVMGenerator.hpp"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"

CodeGenContext::CodeGenContext()
    : builder(context), module("hulk_module", context), typeSystem(&context, &module) {}  // Initialize TypeSystem with LLVM context and module

void CodeGenContext::generateCode(std::vector<ASTNode*>& root) {
    
    // Separate type declarations, function declarations, and expressions
    std::vector<ASTNode*> typeDecls;
    std::vector<ASTNode*> funcDecls;
    std::vector<ASTNode*> exprs;

    for (ASTNode* node : root) {
        if (auto* typeDecl = dynamic_cast<TypeDeclarationNode*>(node)) {
            typeDecls.push_back(node);
        } else if (auto* fn = dynamic_cast<FunctionDeclarationNode*>(node)) {
            funcDecls.push_back(node);
        } else {
            exprs.push_back(node);
        }
    }

    // Process type declarations first - register types and generate LLVM types
    pushFuncScope();  // Global function registry
    LLVMGenerator generator(*this);
    
    // First pass: Register all types
    for (ASTNode* node : typeDecls) {
        node->accept(generator);
    }
    
    // Second pass: Generate LLVM types for all registered types
    for (ASTNode* node : typeDecls) {
        if (auto* typeDecl = dynamic_cast<TypeDeclarationNode*>(node)) {
            typeSystem.generateLLVMTypes(typeDecl->name);
        }
    }

    // Then process function declarations
    for (ASTNode* node : funcDecls) {
        if (auto* fn = dynamic_cast<FunctionDeclarationNode*>(node)) {
            addFuncDecl(fn->name, fn);
        }
    }

    // Finally process expressions
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

    // Generate code for all remaining nodes
    for (ASTNode* node : root) {
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

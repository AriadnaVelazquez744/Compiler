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
    std::cerr << "[DEBUG] Starting generateCode" << std::endl;
    
    // Create main function and set up builder context FIRST
    LLVMGenerator generator(*this);
    generator.setupMainFunction();
    
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
    std::cerr << "[DEBUG] Collected typeDecls: " << typeDecls.size() << ", funcDecls: " << funcDecls.size() << ", exprs: " << exprs.size() << std::endl;
    
    // Process type declarations first - register types and generate LLVM types
    pushFuncScope();  // Global function registry
    
    // First pass: Register all types
    for (ASTNode* node : typeDecls) {
        std::cerr << "[DEBUG] Registering typeDecl" << std::endl;
        node->accept(generator);
    }
    std::cerr << "[DEBUG] Finished registering types" << std::endl;
    
    // Second pass: Generate LLVM types for all registered types
    for (ASTNode* node : typeDecls) {
        if (auto* typeDecl = dynamic_cast<TypeDeclarationNode*>(node)) {
            std::cerr << "[DEBUG] Generating LLVM types for: " << typeDecl->name << std::endl;
            typeSystem.generateLLVMTypes(typeDecl->name);
        }
    }
    std::cerr << "[DEBUG] Finished generating LLVM types" << std::endl;
    
    // Then process function declarations
    for (ASTNode* node : funcDecls) {
        if (auto* fn = dynamic_cast<FunctionDeclarationNode*>(node)) {
            std::cerr << "[DEBUG] Adding function declaration: " << fn->name << std::endl;
            addFuncDecl(fn->name, fn);
        }
    }
    std::cerr << "[DEBUG] Finished registering functions" << std::endl;
    
    // Finally process expressions (now inside main function)
    root = std::move(exprs);
    std::cerr << "[DEBUG] Generating code for expressions" << std::endl;
    for (ASTNode* node : root) {
        std::cerr << "[DEBUG] Generating code for expression node" << std::endl;
        node->accept(generator);
    }
    std::cerr << "[DEBUG] Finished generating code for expressions" << std::endl;
    
    // Print all remaining values on the stack
    for (llvm::Value* val : valueStack) {
        std::cerr << "[DEBUG] Printing value from valueStack" << std::endl;
        if (!val) {
            std::cerr << "[WARNING] Null value on stack, skipping" << std::endl;
            continue;
        }
        
        std::cerr << "[DEBUG] Value type: " << val->getType()->getTypeID() << std::endl;
        
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
        } else {
            std::cerr << "[WARNING] Unknown value type on stack, skipping" << std::endl;
        }
    }
    std::cerr << "[DEBUG] Finished printing valueStack" << std::endl;
    valueStack.clear();
    
    // Return 0 from main function
    builder.CreateRet(llvm::ConstantInt::get(context, llvm::APInt(32, 0)));
    
    // Verify the main function
    llvm::verifyFunction(*module.getFunction("main"));
    std::cerr << "[DEBUG] Finished generateCode" << std::endl;
}

void CodeGenContext::dumpIR(const std::string& filename) {
    std::cerr << "[DEBUG] About to dump IR to " << filename << std::endl;
    std::error_code EC;
    llvm::raw_fd_ostream out(filename, EC);
    if (EC) {
        std::cerr << "[ERROR] Could not open file: " << EC.message() << std::endl;
        return;
    }
    std::cerr << "[DEBUG] About to print module" << std::endl;
    module.print(out, nullptr); // Emit IR
    out.flush();
    std::cerr << "[DEBUG] Finished dumping IR to " << filename << std::endl;
}

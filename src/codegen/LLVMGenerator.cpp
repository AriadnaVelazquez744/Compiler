#include "LLVMGenerator.hpp"
#include "CodeGenContext.hpp"
#include "../ast/AST.hpp"
#include <stdexcept>
#include <iostream>
#include <string>
#include <cstring>
#include <math.h>

extern "C" {
    char* hulk_str_concat(const char* a, const char* b);
    char* hulk_str_concat_space(const char* a, const char* b);
    bool hulk_str_equals(const char* a, const char* b);
    double hulk_log_base_checked(double x, double base);
    double hulk_rand();
}

LLVMGenerator::LLVMGenerator(CodeGenContext& ctx)
    : context(ctx) {}

std::string processRawString(const std::string& raw) {
    std::string processed;
    bool escape = false;
    size_t start = (raw.front() == '"') ? 1 : 0;
    size_t end = (raw.back() == '"') ? raw.size() -1 : raw.size();

    for (size_t i = start; i <= end; ++i) { // Include last character
        char c = raw[i];
        if (escape) {
            switch (c) {
                case 'n':  processed += '\n'; break;
                case 't':  processed += '\t'; break;
                case '"':  processed += '"';  break;
                case '\\': processed += '\\'; break;
                default:   processed += c; // Handle unknown escapes
            }
            escape = false;
        } else if (c == '\\') {
            escape = true;
        } else {
            processed += c;
        }
    }

    // Handle trailing escape character (e.g., raw ends with '\')
    if (escape) {
        processed += '\\';
    }

    return processed;
}

void LLVMGenerator::visit(LiteralNode& node) {
    llvm::Value* val = nullptr;

    if (node._type == "Number") {
        // Convert string to double and emit LLVM float constant
        double numVal = std::stod(node.value);
        val = llvm::ConstantFP::get(context.context, llvm::APFloat(numVal));
    }
    else if (node._type == "Boolean") {
        // Emit 1-bit LLVM integer (i1)
        bool b = (node.value == "true");
        val = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context.context), b);
    }
    else if (node._type == "String") {
        // Emit LLVM constant pointer to global string
        std::string processed = processRawString(node.value);
        val = context.builder.CreateGlobalStringPtr(processed);
    }
    else if (node._type == "Null") {
        // Null pointer constant (void*)
        val = llvm::ConstantPointerNull::get(llvm::PointerType::get(llvm::Type::getInt8Ty(context.context), 0));
    }
    else {
        throw std::runtime_error("Unsupported literal type: " + node._type);
    }

    // Store result on the value stack for parent nodes to use
    context.valueStack.push_back(val);

    std::cout << "🔧 Emitted literal of type " << node._type << std::endl;
}

void LLVMGenerator::visit(BinaryOpNode& node) {
    // 1. Generate left and right values recursively
    node.left->accept(*this);
    llvm::Value* left = context.valueStack.back(); context.valueStack.pop_back();
    node.right->accept(*this);
    llvm::Value* right = context.valueStack.back(); context.valueStack.pop_back();

    llvm::Value* result = nullptr;
    llvm::IRBuilder<>& builder = context.builder;

    const std::string& op = node.op;

    // 2. Handle arithmetic operations for Number
    if (op == "+") {
        result = builder.CreateFAdd(left, right, "addtmp");
    } else if (op == "-") {
        result = builder.CreateFSub(left, right, "subtmp");
    } else if (op == "*") {
        result = builder.CreateFMul(left, right, "multmp");
    } else if (op == "/") {
        result = builder.CreateFDiv(left, right, "divtmp");
    } else if (op == "%") {
        result = builder.CreateFRem(left, right, "modtmp");
    } else if (op == "^") {
        // LLVM has no native power op; use `llvm.pow`
        llvm::Function* powFn = context.module.getFunction("llvm.pow.f64");
        if (!powFn) {
            llvm::FunctionType* powType = llvm::FunctionType::get(
                llvm::Type::getDoubleTy(context.context),
                {llvm::Type::getDoubleTy(context.context), llvm::Type::getDoubleTy(context.context)},
                false
            );
            powFn = llvm::Function::Create(powType, llvm::Function::ExternalLinkage, "llvm.pow.f64", context.module);
        }
        result = builder.CreateCall(powFn, {left, right}, "powtmp");
    }

    // 3. Comparison operators → return Boolean (i1)
    else if (op == "<") {
        result = builder.CreateFCmpULT(left, right, "lttmp");
    } else if (op == "<=") {
        result = builder.CreateFCmpULE(left, right, "letmp");
    } else if (op == ">") {
        result = builder.CreateFCmpUGT(left, right, "gttmp");
    } else if (op == ">=") {
        result = builder.CreateFCmpUGE(left, right, "getmp");
    
    } else if (op == "==" || op == "!=") {
        const std::string& nodeType = node.left->type();

        if (nodeType == "Number") {
            result = (op == "==")
                ? builder.CreateFCmpUEQ(left, right, "eqtmp")
                : builder.CreateFCmpUNE(left, right, "netmp");
        }
        else if (nodeType == "Boolean") {
            result = (op == "==")
                ? builder.CreateICmpEQ(left, right, "beqtmp")
                : builder.CreateICmpNE(left, right, "bnetmp");
        }
        else if (nodeType == "String") {
            // Call external C function: bool hulk_str_equals(const char*, const char*)
            llvm::Function* eqFn = context.module.getFunction("hulk_str_equals");
            if (!eqFn) {
                llvm::FunctionType* eqType = llvm::FunctionType::get(
                    llvm::Type::getInt1Ty(context.context), // return type: i1
                    {
                        llvm::PointerType::get(llvm::Type::getInt8Ty(context.context), 0), // const char*
                        llvm::PointerType::get(llvm::Type::getInt8Ty(context.context), 0)  // const char*
                    },
                    false
                );
                eqFn = llvm::Function::Create(
                    eqType, llvm::Function::ExternalLinkage,
                    "hulk_str_equals", context.module
                );
            }

            llvm::Value* eqCall = builder.CreateCall(eqFn, {left, right}, "strequal");

            result = (op == "!=")
                ? builder.CreateNot(eqCall, "strnotequal")
                : eqCall;
        }
        else {
            throw std::runtime_error("❌ Unsupported type for '==' or '!=': " + nodeType);
        }
    }


    else if (op == "&") {
        result = builder.CreateAnd(left, right, "andtmp");
    } else if (op == "|") {
        result = builder.CreateOr(left, right, "ortmp");
    }

    else if (op == "@" || op == "@@") {
        const char* funcName = (op == "@") ? "hulk_str_concat" : "hulk_str_concat_space";

        // Declare function if not already in module
        llvm::Function* concatFn = context.module.getFunction(funcName);
        if (!concatFn) {
            llvm::FunctionType* concatType = llvm::FunctionType::get(
                llvm::PointerType::get(llvm::Type::getInt8Ty(context.context), 0),
                {llvm::PointerType::get(llvm::Type::getInt8Ty(context.context), 0), llvm::PointerType::get(llvm::Type::getInt8Ty(context.context), 0)},
                false
            );
            concatFn = llvm::Function::Create(concatType, llvm::Function::ExternalLinkage, funcName, context.module);
        }

        // Emit call
        result = context.builder.CreateCall(concatFn, {left, right}, "concat");
    }


    else {
        throw std::runtime_error("❌ Unsupported binary operator: " + op);
    }

    // 6. Push the result onto the stack
    context.valueStack.push_back(result);
    std::cout << "🔧 Binary op '" << op << "' emitted.\n";
}

void LLVMGenerator::visit(UnaryOpNode& node) {
    // 1. Visit operand first
    node.operand->accept(*this);
    llvm::Value* operand = context.valueStack.back();
    context.valueStack.pop_back();

    llvm::Value* result = nullptr;
    llvm::IRBuilder<>& builder = context.builder;

    const std::string& op = node.op;

    if (op == "-") {
        // Numeric negation: emit 0.0 - operand
        llvm::Value* zero = llvm::ConstantFP::get(context.context, llvm::APFloat(0.0));
        result = builder.CreateFSub(zero, operand, "negtmp");
    }
    else if (op == "!") {
        // Logical NOT: i1 → i1
        result = builder.CreateNot(operand, "nottmp");
    }
    else {
        throw std::runtime_error("❌ Unsupported unary operator: " + op);
    }

    context.valueStack.push_back(result);
    std::cout << "🔧 Unary op '" << op << "' emitted.\n";
}

void LLVMGenerator::visit(BuiltInFunctionNode& node) {
    llvm::IRBuilder<>& builder = context.builder;
    std::vector<llvm::Value*> args;

    for (ASTNode* arg : node.args) {
        arg->accept(*this);
        args.push_back(context.valueStack.back());
        context.valueStack.pop_back();
    }

    llvm::Value* result = nullptr;
    std::string name = node.name;

    auto emit_llvm_unary_math = [&](const std::string& fnName, llvm::Value* arg) {
        llvm::Function* fn = context.module.getFunction(fnName);
        if (!fn) {
            llvm::FunctionType* ft = llvm::FunctionType::get(
                llvm::Type::getDoubleTy(context.context),
                {llvm::Type::getDoubleTy(context.context)},
                false
            );
            fn = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, fnName, context.module);
        }
        return builder.CreateCall(fn, {arg});
    };

    if (name == "print") {
        result = args[0];
    }
    else if (name == "sin" || name == "cos" || name == "sqrt" || name == "exp") {
        std::string fnName = "llvm." + name + ".f64";
        result = emit_llvm_unary_math(fnName, args[0]);
    }
    else if (name == "min") {
        llvm::Function* fn = context.module.getFunction("fmin");
        if (!fn) {
            llvm::FunctionType* type = llvm::FunctionType::get(
                llvm::Type::getDoubleTy(context.context),
                {llvm::Type::getDoubleTy(context.context), llvm::Type::getDoubleTy(context.context)},
                false
            );
            fn = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "fmin", context.module);
        }
        result = builder.CreateCall(fn, {args[0], args[1]}, "mincall");
    }
    else if (name == "max") {
        llvm::Function* fn = context.module.getFunction("fmax");
        if (!fn) {
            llvm::FunctionType* type = llvm::FunctionType::get(
                llvm::Type::getDoubleTy(context.context),
                {llvm::Type::getDoubleTy(context.context), llvm::Type::getDoubleTy(context.context)},
                false
            );
            fn = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "fmax", context.module);
        }
        result = builder.CreateCall(fn, {args[0], args[1]}, "maxcall");
    }
    else if (name == "log") {
        llvm::Function* logFn = context.module.getFunction("hulk_log_base_checked");
        if (!logFn) {
            llvm::FunctionType* ft = llvm::FunctionType::get(
                llvm::Type::getDoubleTy(context.context),
                {llvm::Type::getDoubleTy(context.context), llvm::Type::getDoubleTy(context.context)},
                false
            );
            logFn = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "hulk_log_base_checked", context.module);
        }
        result = builder.CreateCall(logFn, {args[0], args[1]}, "logcall");
    }
    else if (name == "rand") {
        llvm::Function* randFn = context.module.getFunction("hulk_rand");
        if (!randFn) {
            llvm::FunctionType* ft = llvm::FunctionType::get(
                llvm::Type::getDoubleTy(context.context), false
            );
            randFn = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "hulk_rand", context.module);
        }
        result = builder.CreateCall(randFn, {}, "randcall");
    }
    else {
        throw std::runtime_error("❌ Unsupported built-in function: " + name);
    }

    if (result) {
        context.valueStack.push_back(result);
    }

    std::cout << "🔧 Built-in function '" << name << "' emitted.\n";
}

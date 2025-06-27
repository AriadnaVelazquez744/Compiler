#include "LLVMGenerator.hpp"
#include "CodeGenContext.hpp"
#include "../ast/AST.hpp"
#include <stdexcept>
#include <iostream>
#include <string>
#include <cstring>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.7182818284590452354
#endif

extern "C" {
    char* hulk_str_concat(const char* a, const char* b);
    char* hulk_str_concat_space(const char* a, const char* b);
    bool hulk_str_equals(const char* a, const char* b);
    double hulk_log_base_checked(double x, double base);
    double hulk_rand();
}

LLVMGenerator::LLVMGenerator(CodeGenContext& ctx)
    : context(ctx) {}

llvm::IRBuilder<>& LLVMGenerator::getBuilder() { 
    return context.builder; 
}

void LLVMGenerator::setBuilder(llvm::IRBuilder<>* newBuilder) { 
    // Note: This is a simplified approach. In a real implementation,
    // you'd want to properly manage the builder context
    (void)newBuilder; // Suppress unused parameter warning
}

std::vector<llvm::Value*>& LLVMGenerator::getValueStack() { 
    return context.valueStack; 
}

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
    std::cout << "🔍 BinaryOp: " << node.op << " - Stack size before: " << context.valueStack.size() << std::endl;

    // 1. Generate left and right values recursively
    node.left->accept(*this);
    llvm::Value* left = context.valueStack.back(); context.valueStack.pop_back();
    std::cout << "  📥 Left operand evaluated - Stack size: " << context.valueStack.size() << std::endl;

    node.right->accept(*this);
    llvm::Value* right = context.valueStack.back(); context.valueStack.pop_back();
    std::cout << "  📥 Right operand evaluated - Stack size: " << context.valueStack.size() << std::endl;

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
        std::cout << "🔍 String concatenation with " << funcName << std::endl;

        // Convert numbers to strings if needed
        llvm::Value* leftStr = left;
        llvm::Value* rightStr = right;

        // Helper function to convert number to string
        auto convertToString = [&](llvm::Value* val) -> llvm::Value* {
            if (val->getType()->isDoubleTy()) {
                // Get the sprintf function
                llvm::Function* sprintfFn = context.module.getFunction("sprintf");
                if (!sprintfFn) {
                    llvm::FunctionType* sprintfType = llvm::FunctionType::get(
                        llvm::Type::getInt32Ty(context.context),
                        {
                            llvm::PointerType::get(llvm::Type::getInt8Ty(context.context), 0),
                            llvm::PointerType::get(llvm::Type::getInt8Ty(context.context), 0)
                        },
                        true
                    );
                    sprintfFn = llvm::Function::Create(
                        sprintfType,
                        llvm::Function::ExternalLinkage,
                        "sprintf",
                        context.module
                    );
                }

                // Allocate buffer for the string
                llvm::Value* buffer = context.builder.CreateAlloca(
                    llvm::Type::getInt8Ty(context.context),
                    llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.context), 32)
                );

                // Format string for double
                llvm::Value* formatStr = context.builder.CreateGlobalStringPtr("%g");

                // Call sprintf
                std::vector<llvm::Value*> args = {buffer, formatStr, val};
                context.builder.CreateCall(sprintfFn, args);

                return buffer;
            }
            return val;
        };

        // Convert operands if they are numbers
        leftStr = convertToString(left);
        rightStr = convertToString(right);

        // Declare concatenation function if not already in module
        llvm::Function* concatFn = context.module.getFunction(funcName);
        if (!concatFn) {
            llvm::FunctionType* concatType = llvm::FunctionType::get(
                llvm::PointerType::get(llvm::Type::getInt8Ty(context.context), 0),
                {
                    llvm::PointerType::get(llvm::Type::getInt8Ty(context.context), 0),
                    llvm::PointerType::get(llvm::Type::getInt8Ty(context.context), 0)
                },
                false
            );
            concatFn = llvm::Function::Create(concatType, llvm::Function::ExternalLinkage, funcName, context.module);
        }

        // Emit call with converted operands
        result = context.builder.CreateCall(concatFn, {leftStr, rightStr}, "concat");
    }


    else {
        throw std::runtime_error("❌ Unsupported binary operator: " + op);
    }

    // 6. Push the result onto the stack
    context.valueStack.push_back(result);
    std::cout << "  📤 Result pushed to stack - Final stack size: " << context.valueStack.size() << std::endl;
    std::cout << "✅ Binary op '" << op << "' processed" << std::endl;
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

    std::cout << "🔍 BuiltInFunction: " << node.name << " - Stack size before args: " << context.valueStack.size() << std::endl;

    for (ASTNode* arg : node.args) {
        arg->accept(*this);
        args.push_back(context.valueStack.back());
        context.valueStack.pop_back();
        std::cout << "  📥 Arg evaluated - Stack size: " << context.valueStack.size() << std::endl;
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
        std::cout << "  🖨️ Processing print statement" << std::endl;
        
        llvm::Value* valueToPrint = args[0];
        
        // Handle different types
        if (valueToPrint->getType()->isDoubleTy()) {
            // Print number with printf
            llvm::Function* printfFn = context.module.getFunction("printf");
            if (!printfFn) {
                llvm::FunctionType* printfType = llvm::FunctionType::get(
                    llvm::Type::getInt32Ty(context.context),
                    {llvm::PointerType::get(llvm::Type::getInt8Ty(context.context), 0)},
                    true
                );
                printfFn = llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", context.module);
            }
            llvm::Value* formatStr = context.builder.CreateGlobalStringPtr("%g\n");
            context.builder.CreateCall(printfFn, {formatStr, valueToPrint});
        } else if (valueToPrint->getType()->isIntegerTy(1)) {
            // Print boolean
            llvm::Function* putsFn = context.module.getFunction("puts");
            if (!putsFn) {
                llvm::FunctionType* putsType = llvm::FunctionType::get(
                    llvm::Type::getInt32Ty(context.context),
                    {llvm::PointerType::get(llvm::Type::getInt8Ty(context.context), 0)},
                    false
                );
                putsFn = llvm::Function::Create(putsType, llvm::Function::ExternalLinkage, "puts", context.module);
            }
            llvm::Value* str = context.builder.CreateSelect(
                valueToPrint,
                context.builder.CreateGlobalStringPtr("true\n"),
                context.builder.CreateGlobalStringPtr("false\n")
            );
            context.builder.CreateCall(putsFn, {str});
        } else if (valueToPrint->getType()->isPointerTy()) {
            // Print string
            llvm::Function* putsFn = context.module.getFunction("puts");
            if (!putsFn) {
                llvm::FunctionType* putsType = llvm::FunctionType::get(
                    llvm::Type::getInt32Ty(context.context),
                    {llvm::PointerType::get(llvm::Type::getInt8Ty(context.context), 0)},
                    false
                );
                putsFn = llvm::Function::Create(putsType, llvm::Function::ExternalLinkage, "puts", context.module);
            }
            context.builder.CreateCall(putsFn, {valueToPrint});
        }
        
        // Print function returns void (no result) - don't push anything to stack
        result = nullptr;
        std::cout << "  📤 Print completed - no value returned" << std::endl;
        return; // Exit early to avoid pushing result to stack
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
        std::cout << "  📤 Result pushed to stack - Final stack size: " << context.valueStack.size() << std::endl;
    }

    std::cout << "🔧 Built-in function '" << name << "' emitted.\n";
}

void LLVMGenerator::visit(BlockNode& node) {
    if (node.expressions.empty()) {
        throw std::runtime_error("❌ Block must contain at least one expression (line " + std::to_string(node.line()) + ")");
    }

    std::cout << "🔍 BlockNode - Stack size before: " << context.valueStack.size() << std::endl;
    context.pushFuncScope();

    // Separate declarations from other expressions
    std::vector<ASTNode*> bodyExprs;
    for (ASTNode* expr : node.expressions) {
        if (auto* decl = dynamic_cast<FunctionDeclarationNode*>(expr)) {
            context.addFuncDecl(decl->name, decl); // Register locally
            std::cout << "  📝 Registered function: " << decl->name << std::endl;
        } else {
            bodyExprs.push_back(expr);
        }
    }

    llvm::Value* lastValidResult = nullptr;

    for (size_t i = 0; i < bodyExprs.size(); ++i) {
        ASTNode* expr = bodyExprs[i];
        std::cout << "  🔄 Evaluating expression " << i + 1 << "/" << bodyExprs.size() << " - Stack size: " << context.valueStack.size() << std::endl;
        expr->accept(*this);

        bool isPrint = false;
        if (auto* builtin = dynamic_cast<BuiltInFunctionNode*>(expr)) {
            isPrint = (builtin->name == "print");
        }

        // Handle result from stack if present
        if (!context.valueStack.empty()) {
            llvm::Value* val = context.valueStack.back();

            if (i == bodyExprs.size() - 1) {
                // Last expression in block
                if (isPrint) {
                    // Duplicate value: keep both for print + return
                    context.valueStack.push_back(val);
                    lastValidResult = val;
                } else {
                    lastValidResult = val;
                }
            } else {
                if (!isPrint) {
                    context.valueStack.pop_back(); // discard intermediate result
                } else {
                    lastValidResult = val;
                    // Keep value in stack for print
                }
            }
        }
        std::cout << "  📤 Expression " << i + 1 << " completed - Stack size: " << context.valueStack.size() << std::endl;
    }

    context.popFuncScope();

    if (!lastValidResult) {
        throw std::runtime_error("❌ Block has no returnable value on last expression (line " + std::to_string(node.line()) + ")");
    }

    std::cout << "  ✅ BlockNode completed - Final stack size: " << context.valueStack.size() << std::endl;
}

void LLVMGenerator::visit(IdentifierNode& node) {
    // Handle mathematical constants
    if (node.name == "PI" || node.name == "pi") {
        llvm::Value* pi = llvm::ConstantFP::get(context.context, llvm::APFloat(M_PI));
        context.valueStack.push_back(pi);
        std::cout << "🔧 Mathematical constant PI emitted\n";
        return;
    }
    else if (node.name == "E" || node.name == "e") {
        llvm::Value* e = llvm::ConstantFP::get(context.context, llvm::APFloat(M_E));
        context.valueStack.push_back(e);
        std::cout << "🔧 Mathematical constant E emitted\n";
        return;
    }

    // Handle regular variables
    llvm::Value* val = context.lookupLocal(node.name);

    if (!val) {
        throw std::runtime_error("❌ Undefined variable '" + node.name +
                                 "' at line " + std::to_string(node.line()));
    }

    context.valueStack.push_back(val);

    std::cout << "🔧 Identifier '" << node.name << "' resolved and pushed to stack\n";
}

void LLVMGenerator::visit(FunctionDeclarationNode& node) {
    std::cout << "🔍 FunctionDecl: " << node.name << " - Stack size before params: " << context.valueStack.size() << std::endl;
    
    context.pushVarScope(false);  // Create an isolated scope for function parameters

    const auto& params = *node.params;
    if (params.size() > context.valueStack.size()) {
        throw std::runtime_error("❌ Not enough arguments on stack for function '" + node.name + "'");
    }

    // Pop values in reverse and bind to parameter names
    for (int i = params.size() - 1; i >= 0; --i) {
        llvm::Value* val = context.valueStack.back();
        context.valueStack.pop_back();
        context.addLocal(params[i].name, val);
        std::cout << "  📥 Bound param " << params[i].name << " - Stack size: " << context.valueStack.size() << std::endl;
    }

    // Evaluate body and leave result on stack
    std::cout << "  🔄 Evaluating function body - Stack size: " << context.valueStack.size() << std::endl;
    node.body->accept(*this);
    std::cout << "  📤 Body evaluated - Stack size: " << context.valueStack.size() << std::endl;

    // If the body didn't leave a value on the stack, we need to handle that
    if (context.valueStack.empty()) {
        std::cout << "  ⚠️ No return value, pushing null" << std::endl;
        context.valueStack.push_back(
            llvm::ConstantPointerNull::get(llvm::PointerType::get(llvm::Type::getInt8Ty(context.context), 0))
        );
    }

    context.popVarScope();
    std::cout << "  ✅ Function " << node.name << " completed - Final stack size: " << context.valueStack.size() << std::endl;
}

void LLVMGenerator::visit(LetNode& node) {
    std::cerr << "[DEBUG] LLVMGenerator::visit(LetNode) - Stack size: " << context.valueStack.size() << std::endl;
    if (node.declarations) {
        for (const auto& decl : *node.declarations) {
            std::cerr << "[DEBUG] LetNode declaration: name='" << decl.name << "'";
            if (decl.initializer) {
                std::cerr << ", has initializer";
            } else {
                std::cerr << ", no initializer";
            }
            std::cerr << std::endl;
        }
    }
    
    // 1. Initialize new variable scope with inheritance
    context.pushVarScope(true);  // true for inheritance

    // 2. Process each declaration
    for (const LetDeclaration& decl : *node.declarations) {
        // Push the variable name and type onto the placeholder stack before processing its value
        context.typeSystem.pushPlaceholder(decl.name, "var");
        std::cout << "  📝 Added placeholder: " << decl.name << " of type var" << std::endl;

        // Process the initializer
        decl.initializer->accept(*this);
        
        // Get the value from the stack (whether it's a NewInstanceNode or other type)
        if (context.valueStack.empty()) {
            throw std::runtime_error("❌ Initializer for variable '" + decl.name + "' produced no value");
        }
        llvm::Value* initValue = context.valueStack.back();
        context.valueStack.pop_back();

        // Add the variable to the current scope
        context.addLocal(decl.name, initValue);
        std::cout << "  📝 Added variable '" << decl.name << "' to scope" << std::endl;

        // Register the type in the type system if this is a NewInstanceNode
        if (auto* newInstance = dynamic_cast<NewInstanceNode*>(decl.initializer)) {
            context.typeSystem.createInstance(decl.name, newInstance->typeName, {});
            std::cout << "  📝 Registered instance type: '" << decl.name << "' as '" << newInstance->typeName << "'" << std::endl;
        }

        // Pop the placeholder after processing
        context.typeSystem.popPlaceholder();
    }

    // 3. Process the body
    std::cout << "  🔄 Processing let body - Stack size: " << context.valueStack.size() << std::endl;
    node.body->accept(*this);
    std::cout << "  📤 Body processed - Stack size: " << context.valueStack.size() << std::endl;

    // 4. Keep the body's return value on the stack
    // The value is already on the stack from the body processing

    // Clean up the scope
    context.popVarScope();
    std::cout << "  ✅ LetNode completed - Final stack size: " << context.valueStack.size() << std::endl;
    std::cerr << "[DEBUG] Exiting LetNode - Stack size after: " << context.valueStack.size() << std::endl;
}

void LLVMGenerator::visit(FunctionCallNode& node) {
    std::cout << "🔍 FunctionCall: " << node.funcName << " - Stack size before args: " << context.valueStack.size() << std::endl;
    
    // Evaluate each argument (left-to-right) and push onto stack
    for (ASTNode* arg : node.args) {
        arg->accept(*this);
        std::cout << "  📥 Arg evaluated - Stack size: " << context.valueStack.size() << std::endl;
    }

    // Lookup FunctionDeclarationNode (not LLVM::Function)
    auto* decl = context.lookupFuncDecl(node.funcName);
    if (!decl) {
        throw std::runtime_error("❌ Function not declared: " + node.funcName);
    }

    std::cout << "  🔄 Calling function body - Stack size: " << context.valueStack.size() << std::endl;
    // Process the function body using the declaration
    decl->accept(*this);  // Will consume args from stack, push result
    std::cout << "  📤 Function returned - Stack size: " << context.valueStack.size() << std::endl;
}

void LLVMGenerator::visit(AssignmentNode& node) {
    std::cout << "🔍 AssignmentNode - Stack size before: " << context.valueStack.size() << std::endl;

    // 1. Process the right-hand side value first
    node.rhs->accept(*this);
    llvm::Value* newValue = context.valueStack.back();
    context.valueStack.pop_back();

    // 2. Get the variable name from the left-hand side
    if (auto* idNode = dynamic_cast<IdentifierNode*>(node.name)) {
        const std::string& varName = idNode->name;
        bool found = false;

        // 3. Search through all scopes from innermost to outermost
        for (auto it = context.localScopes.rbegin(); it != context.localScopes.rend(); ++it) {
            auto foundVar = it->find(varName);
            if (foundVar != it->end()) {
                // Found the variable in this scope, update its value
                foundVar->second = newValue;
                found = true;
            } else if (found) {
                // If we previously found the variable but now we don't, stop searching
                break;
            }
        }

        if (!found) {
            throw std::runtime_error("❌ Undefined variable '" + varName + 
                                   "' at line " + std::to_string(node.line()));
        }

        // 5. Push the assigned value onto the stack
        context.valueStack.push_back(newValue);
        std::cout << "  ✅ Variable '" << varName << "' assigned in all relevant scopes - Stack size: " << context.valueStack.size() << std::endl;
    } else {
        // Handle instance variable assignment
        std::string varName;
        
        // 1. Get the variable name
        if (auto* idNode = dynamic_cast<IdentifierNode*>(node.name)) {
            varName = idNode->name;
        } else if (auto* selfNode = dynamic_cast<SelfCallNode*>(node.name)) {
            varName = selfNode->varName;
        } else {
            throw std::runtime_error("❌ Left-hand side of assignment must be an identifier or self access at line " + 
                                   std::to_string(node.line()));
        }

        // 2. Look for the variable in currentInstanceVars
        if (!context.typeSystem.isInstanceVarsStackEmpty()) {
            // 3. Change its value for newValue
            context.typeSystem.setCurrentInstanceVar(varName, "var", newValue);
            std::cout << "  ✅ Instance variable '" << varName << "' assigned - Stack size: " << context.valueStack.size() << std::endl;
        } else {
            throw std::runtime_error("❌ Cannot assign to instance variable outside of type context at line " + 
                                   std::to_string(node.line()));
        }

        // Push the assigned value onto the stack
        context.valueStack.push_back(newValue);
    }
}

void LLVMGenerator::visit(IfNode& node) {
    std::cout << "🔍 IfNode - Stack size before: " << context.valueStack.size() << std::endl;
    
    // Process each branch (if + elif)
    for (const IfBranch& branch : *node.branches) {
        // Evaluate condition
        branch.condition->accept(*this);
        llvm::Value* condition = context.valueStack.back();
        context.valueStack.pop_back();

        // Direct comparison with true (1)
        if (auto* constInt = llvm::dyn_cast<llvm::ConstantInt>(condition)) {
            if (constInt->getZExtValue() == 1) {
                // Condition is true, evaluate body
                branch.body->accept(*this);
                return;
            }
        }
    }

    // If we get here, no condition was true, evaluate else body
    if (node.elseBody) {
        node.elseBody->accept(*this);
    }

    std::cout << "✅ IfNode completed - Final stack size: " << context.valueStack.size() << std::endl;
}

void LLVMGenerator::visit(WhileNode& node) {
    std::cout << "🔍 WhileNode - Stack size before: " << context.valueStack.size() << std::endl;

    // Create a new scope for the loop
    context.pushVarScope(true);

    // Create a vector to store loop body values
    std::vector<llvm::Value*> loopBodyValues;

    int iteration = 0;
    // Start the loop
    while (true) {
        // Evaluate condition
        node.condition->accept(*this);
        llvm::Value* condition = context.valueStack.back();
        context.valueStack.pop_back();

        // Direct comparison with true (1)
        if (auto* constInt = llvm::dyn_cast<llvm::ConstantInt>(condition)) {
            if (constInt->getZExtValue() == 1) {
                // Condition is true, evaluate body
                std::cout << "🔄 Iteration " << ++iteration << std::endl;
                node.body->accept(*this);
                
                // If body produced a value, store it
                if (!context.valueStack.empty()) {
                    llvm::Value* bodyValue = context.valueStack.back();
                    context.valueStack.pop_back(); // Remove from global stack
                    loopBodyValues.push_back(bodyValue);
                }
            } else {
                // Condition is false, exit loop
                break;
            }
        } else {
            // If not a constant, convert to boolean and check
            condition = context.builder.CreateFCmpONE(
                condition,
                llvm::ConstantFP::get(context.context, llvm::APFloat(0.0)),
                "whilecond"
            );
            
            if (auto* boolVal = llvm::dyn_cast<llvm::ConstantInt>(condition)) {
                if (boolVal->getZExtValue() == 1) {
                    // Condition is true, evaluate body
                    std::cout << "🔄 Iteration " << ++iteration << std::endl;
                    node.body->accept(*this);
                    
                    // If body produced a value, store it
                    if (!context.valueStack.empty()) {
                        llvm::Value* bodyValue = context.valueStack.back();
                        context.valueStack.pop_back(); // Remove from global stack
                        loopBodyValues.push_back(bodyValue);
                    }
                } else {
                    // Condition is false, exit loop
                    break;
                }
            }
        }
    }

    std::cout << "✅ Loop ended after " << iteration << " iterations" << std::endl;

    // After loop ends, push the last body value to global stack if any
    if (!loopBodyValues.empty()) {
        context.valueStack.push_back(loopBodyValues.back());
    }

    // Clean up loop scope
    context.popVarScope();

    std::cout << "✅ WhileNode completed - Final stack size: " << context.valueStack.size() << std::endl;
}

void LLVMGenerator::visit(ForNode& node) {
    std::cout << "🔍 ForNode - Stack size before: " << context.valueStack.size() << std::endl;

    // 1. Initialize a varScope with inherit property
    context.pushVarScope(true);

    // 2. Evaluate init_range and end_range expressions
    node.init_range->accept(*this);
    llvm::Value* initValue = context.valueStack.back();
    context.valueStack.pop_back();

    node.end_range->accept(*this);
    llvm::Value* endValue = context.valueStack.back();
    context.valueStack.pop_back();

    // 3. Add the variable varName with initValue to the scope
    context.addLocal(node.varName, initValue);

    // Create a vector to store loop body values
    std::vector<llvm::Value*> loopBodyValues;

    int iteration = 0;
    // 4. Initialize the for loop
    while (true) {
        // Get current value of the loop variable
        llvm::Value* currentValue = context.lookupLocal(node.varName);
        
        // Compare current value with end value
        llvm::Value* condition = context.builder.CreateFCmpULT(
            currentValue,
            endValue,
            "forcond"
        );

        // Check if we should continue the loop
        if (auto* constInt = llvm::dyn_cast<llvm::ConstantInt>(condition)) {
            if (constInt->getZExtValue() == 1) {
                // 5. Evaluate body for this iteration
                std::cout << "🔄 Iteration " << ++iteration << std::endl;
                node.body->accept(*this);
                
                // 6. Store body value if any
                if (!context.valueStack.empty()) {
                    llvm::Value* bodyValue = context.valueStack.back();
                    
                    // Check if this is a print statement
                    bool isPrint = false;
                    if (auto* builtin = dynamic_cast<BuiltInFunctionNode*>(node.body)) {
                        isPrint = (builtin->name == "print");
                    }
                    
                    if (isPrint) {
                        // For print statements, keep value in global stack and also store in local stack
                        loopBodyValues.push_back(bodyValue);
                    } else {
                        // For non-print statements, remove from global stack and store in local stack
                        context.valueStack.pop_back();
                        loopBodyValues.push_back(bodyValue);
                    }
                }

                // Increment loop variable
                llvm::Value* one = llvm::ConstantFP::get(context.context, llvm::APFloat(1.0));
                llvm::Value* nextValue = context.builder.CreateFAdd(currentValue, one, "nextval");
                context.addLocal(node.varName, nextValue);
            } else {
                // Condition is false, exit loop
                break;
            }
        } else {
            // If not a constant, convert to boolean and check
            condition = context.builder.CreateFCmpONE(
                condition,
                llvm::ConstantFP::get(context.context, llvm::APFloat(0.0)),
                "forcond"
            );
            
            if (auto* boolVal = llvm::dyn_cast<llvm::ConstantInt>(condition)) {
                if (boolVal->getZExtValue() == 1) {
                    // 5. Evaluate body for this iteration
                    std::cout << "🔄 Iteration " << ++iteration << std::endl;
                    node.body->accept(*this);
                    
                    // 6. Store body value if any
                    if (!context.valueStack.empty()) {
                        llvm::Value* bodyValue = context.valueStack.back();
                        
                        // Check if this is a print statement
                        bool isPrint = false;
                        if (auto* builtin = dynamic_cast<BuiltInFunctionNode*>(node.body)) {
                            isPrint = (builtin->name == "print");
                        }
                        
                        if (isPrint) {
                            // For print statements, keep value in global stack and also store in local stack
                            loopBodyValues.push_back(bodyValue);
                        } else {
                            // For non-print statements, remove from global stack and store in local stack
                            context.valueStack.pop_back();
                            loopBodyValues.push_back(bodyValue);
                        }
                    }

                    // Increment loop variable
                    llvm::Value* one = llvm::ConstantFP::get(context.context, llvm::APFloat(1.0));
                    llvm::Value* nextValue = context.builder.CreateFAdd(currentValue, one, "nextval");
                    context.addLocal(node.varName, nextValue);
                } else {
                    // Condition is false, exit loop
                    break;
                }
            }
        }
    }

    std::cout << "✅ For loop ended after " << iteration << " iterations" << std::endl;

    // 7. Push the last body value to global stack if any
    if (!loopBodyValues.empty()) {
        context.valueStack.push_back(loopBodyValues.back());
    }

    // Clean up loop scope
    context.popVarScope();

    std::cout << "✅ ForNode completed - Final stack size: " << context.valueStack.size() << std::endl;
}

void LLVMGenerator::visit(TypeDeclarationNode& node) {
    std::cout << "🔍 TypeDeclaration: " << node.name << std::endl;

    // Check if type is already registered
    if (context.typeSystem.typeExists(node.name)) {
        std::cout << "  ⚠️ Type " << node.name << " already registered, skipping" << std::endl;
        return;
    }

    // Register the type
    auto& typeDef = context.typeSystem.registerType(node.name, node.baseType);

    // Set constructor parameters and base args
    typeDef.constructorParams.clear();
    if (node.constructorParams) {
        for (const auto& param : *node.constructorParams) {
            typeDef.constructorParams.push_back(param.name);
        }
    }
    typeDef.baseArgs = node.baseArgs;

    // Set current type for processing attributes and methods
    context.typeSystem.setCurrentType(node.name);

    // Process attributes
    if (node.body->attributes) {
        for (const auto& attr : *node.body->attributes) {
            // Get the attribute type from the symbol table (set by semantic analysis)
            std::string attrType = attr._type;
            if (attrType.empty()) {
                // Try to get the type from the symbol table
                // This should have been set by the semantic analysis
                attrType = "Number"; // Default to Number for constructor parameters
                std::cerr << "[DEBUG] Using default type 'Number' for attribute '" << attr.name << "' in type '" << node.name << "'\n";
            }
            std::cerr << "[DEBUG] Registering attribute '" << attr.name << "' of type '" << attrType << "' in type '" << node.name << "'\n";
            context.typeSystem.addAttribute(node.name, attr.name, attrType, attr.initializer);
            std::cout << "  📝 Added attribute: " << attr.name << " of type " << attrType << std::endl;
        }
    }

    // Process methods
    if (node.body->methods) {
        for (const auto& method : *node.body->methods) {
            context.typeSystem.addMethod(node.name, method.name, method.params, method.body, method.returnType);
            std::cout << "  📝 Added method: " << method.name << std::endl;
        }
    }

    // Generate LLVM types immediately after registration
    context.typeSystem.generateLLVMTypes(node.name);

    std::cout << "✅ Type " << node.name << " registered and LLVM types generated" << std::endl;
}

void LLVMGenerator::visit(NewInstanceNode& node) {
    std::cerr << "[DEBUG] LLVMGenerator::visit(NewInstanceNode) - typeName: " << node.typeName << std::endl;
    if (!context.typeSystem.typeExists(node.typeName)) {
        std::cerr << "[ERROR] NewInstanceNode: type '" << node.typeName << "' does not exist!" << std::endl;
        throw std::runtime_error("Type not found: " + node.typeName);
    }
    auto& typeDef = context.typeSystem.getTypeDefinition(node.typeName);
    std::cerr << "[DEBUG] NewInstanceNode: typeDef pointer: " << &typeDef << ", type name: " << typeDef.name << std::endl;
    std::cerr << "[DEBUG] Entering NewInstanceNode for type: " << node.typeName << std::endl;
    std::cout << "🔍 NewInstance: " << node.typeName << std::endl;

    // Check if we're inside a let declaration
    std::string varName = context.typeSystem.getCurrentPlaceholder().name;
    if (!varName.empty()) {
        std::cout << "  📝 Using placeholder variable: " << varName << std::endl;
    }

    // Check if type exists and has LLVM struct type
    if (!context.typeSystem.typeExists(node.typeName)) {
        throw std::runtime_error("❌ Type '" + node.typeName + "' not found at line " + std::to_string(node.line()));
    }
    if (!typeDef.llvmStructType) {
        std::cerr << "[ERROR] typeDef.llvmStructType is nullptr for type: '" << typeDef.name << "'\n";
        throw std::runtime_error("❌ LLVM struct type not generated for '" + node.typeName + "' at line " + std::to_string(node.line()));
    }
    std::cerr << "[DEBUG] typeDef.llvmStructType pointer: " << typeDef.llvmStructType << std::endl;
    
    // Process constructor arguments first
    std::vector<llvm::Value*> constructorArgs;
    for (ASTNode* arg : node.args) {
        arg->accept(*this);
        if (context.valueStack.empty()) {
            throw std::runtime_error("❌ Failed to evaluate constructor argument at line " + std::to_string(node.line()));
        }
        constructorArgs.push_back(context.valueStack.back());
        context.valueStack.pop_back();
    }
    
    // Create a temporary scope for constructor parameters
    context.pushVarScope(false);
    
    // Bind constructor arguments to parameter names
    if (typeDef.constructorParams.size() != constructorArgs.size()) {
        throw std::runtime_error("❌ Constructor argument count mismatch for type '" + node.typeName + 
                               "': expected " + std::to_string(typeDef.constructorParams.size()) + 
                               ", got " + std::to_string(constructorArgs.size()) + " at line " + std::to_string(node.line()));
    }
    
    for (size_t i = 0; i < typeDef.constructorParams.size(); ++i) {
        context.addLocal(typeDef.constructorParams[i], constructorArgs[i]);
        std::cout << "  📝 Bound constructor param: " << typeDef.constructorParams[i] << std::endl;
    }
    
    // Detailed struct type analysis
    if (typeDef.llvmStructType) {
        std::cerr << "[DEBUG] Struct type is opaque: " << (typeDef.llvmStructType->isOpaque() ? "YES" : "NO") << std::endl;
        unsigned numElements = typeDef.llvmStructType->getNumElements();
        std::cerr << "[DEBUG] typeDef.llvmStructType has " << numElements << " elements" << std::endl;
        for (unsigned i = 0; i < numElements; ++i) {
            llvm::Type* elemType = typeDef.llvmStructType->getElementType(i);
            std::cerr << "[DEBUG] Element " << i << ": " << elemType;
            if (elemType) {
                std::cerr << " (type: " << elemType->getTypeID() << ")";
            } else {
                std::cerr << " (NULL!)";
            }
            std::cerr << std::endl;
        }
    } else {
        std::cerr << "[ERROR] typeDef.llvmStructType is nullptr before allocation!" << std::endl;
    }
    
    std::cerr << "[DEBUG] Before CreateAlloca for type: '" << typeDef.name << "'" << std::endl;
    std::cerr << "[DEBUG] LLVM context: " << &context.context << std::endl;
    std::cerr << "[DEBUG] Builder: " << &context.builder << std::endl;
    
    // Try to get more info about the struct type
    if (typeDef.llvmStructType && !typeDef.llvmStructType->isOpaque()) {
        std::cerr << "[DEBUG] Struct type is valid and not opaque" << std::endl;
        std::cerr << "[DEBUG] Struct type name: " << typeDef.llvmStructType->getName().str() << std::endl;
    } else {
        std::cerr << "[ERROR] Struct type is invalid or opaque!" << std::endl;
    }
    
    llvm::Value* instancePtr = nullptr;
    std::cerr << "[DEBUG] About to call CreateAlloca..." << std::endl;
    try {
        instancePtr = context.builder.CreateAlloca(typeDef.llvmStructType, nullptr, varName + "_ptr");
        std::cerr << "[DEBUG] After CreateAlloca, instancePtr: " << instancePtr << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Exception during CreateAlloca: " << e.what() << std::endl;
        throw;
    } catch (...) {
        std::cerr << "[ERROR] Unknown exception during CreateAlloca" << std::endl;
        throw;
    }
    
    int fieldIndex = 0;
    // Set type ID (first field)
    std::cerr << "[DEBUG] Before CreateStructGEP for typeId at fieldIndex " << fieldIndex << std::endl;
    llvm::Value* typeIdPtr = context.builder.CreateStructGEP(typeDef.llvmStructType, instancePtr, fieldIndex++);
    std::cerr << "[DEBUG] After CreateStructGEP for typeId, typeIdPtr: " << typeIdPtr << std::endl;
    llvm::Value* typeId = llvm::ConstantInt::get(context.context, llvm::APInt(32, typeDef.typeId));
    std::cerr << "[DEBUG] Before CreateStore for typeId" << std::endl;
    context.builder.CreateStore(typeId, typeIdPtr);
    std::cerr << "[DEBUG] After CreateStore for typeId" << std::endl;
    
    // Set vtable pointer (second field)
    std::cerr << "[DEBUG] Before CreateStructGEP for vtable at fieldIndex " << fieldIndex << std::endl;
    if (typeDef.vtableGlobal) {
        llvm::Value* vtablePtr = context.builder.CreateStructGEP(typeDef.llvmStructType, instancePtr, fieldIndex++);
        std::cerr << "[DEBUG] After CreateStructGEP for vtable, vtablePtr: " << vtablePtr << std::endl;
        std::cerr << "[DEBUG] Before CreateStore for vtable" << std::endl;
        context.builder.CreateStore(typeDef.vtableGlobal, vtablePtr);
        std::cerr << "[DEBUG] After CreateStore for vtable" << std::endl;
    } else {
        std::cerr << "[DEBUG] vtableGlobal is nullptr for type: '" << typeDef.name << "'" << std::endl;
        fieldIndex++; // Skip vtable field if not available
    }
    
    // Set parent pointer if inheriting (third field)
    if (typeDef.parentType) {
        std::cerr << "[DEBUG] Before parent pointer logic at fieldIndex " << fieldIndex << std::endl;
        if (!typeDef.parentDef) {
            std::cerr << "[ERROR] parentDef is nullptr for type: '" << typeDef.name << "'\n";
        } else {
            std::cerr << "[DEBUG] parentDef for type '" << typeDef.name << "' is '" << typeDef.parentDef->name << "' at address " << typeDef.parentDef << std::endl;
            if (!typeDef.parentDef->llvmStructType) {
                std::cerr << "[ERROR] parentDef->llvmStructType is nullptr for parent: '" << typeDef.parentDef->name << "'\n";
            } else {
                std::cerr << "[DEBUG] parentDef->llvmStructType for parent '" << typeDef.parentDef->name << "' is at address " << typeDef.parentDef->llvmStructType << std::endl;
            }
        }
        llvm::Type* parentLLVMType = typeDef.parentDef ? typeDef.parentDef->llvmStructType : nullptr;
        std::cerr << "[DEBUG] parentLLVMType for parent pointer: " << parentLLVMType << std::endl;
        std::cerr << "[DEBUG] Before CreateStructGEP for parent pointer" << std::endl;
        llvm::Value* parentPtr = context.builder.CreateStructGEP(typeDef.llvmStructType, instancePtr, fieldIndex);
        std::cerr << "[DEBUG] After CreateStructGEP for parent pointer, parentPtr: " << parentPtr << std::endl;
        // For now, store null (or could recursively create parent instance)
        if (parentLLVMType) {
            std::cerr << "[DEBUG] Before CreateStore for parent pointer (null)" << std::endl;
            context.builder.CreateStore(llvm::ConstantPointerNull::get(llvm::PointerType::get(parentLLVMType, 0)), parentPtr);
            std::cerr << "[DEBUG] After CreateStore for parent pointer (null)" << std::endl;
        } else {
            std::cerr << "[ERROR] parentLLVMType is nullptr, cannot store parent pointer!\n";
        }
        fieldIndex++;
    }
    
    // Print attribute count
    std::cerr << "[DEBUG] typeDef.attributes.size() = " << typeDef.attributes.size() << std::endl;
    
    // Initialize attributes (NOTE: only current type's attributes are initialized here)
    for (const auto& [attrName, attr] : typeDef.attributes) {
        std::cerr << "[DEBUG] Initializing attribute '" << attrName << "' (type: '" << attr.TypeName << "', LLVM type: " << attr.llvmType << ") at fieldIndex " << fieldIndex << std::endl;
        if (attr.initializer) {
            attr.initializer->accept(*this);
            if (context.valueStack.empty()) {
                std::cerr << "[ERROR] valueStack empty after initializing attribute '" << attrName << "'" << std::endl;
                break;
            }
            llvm::Value* attrValue = context.valueStack.back();
            context.valueStack.pop_back();
            llvm::Value* fieldPtr = context.builder.CreateStructGEP(typeDef.llvmStructType, instancePtr, fieldIndex);
            context.builder.CreateStore(attrValue, fieldPtr);
            std::cerr << "[DEBUG] Stored attribute '" << attrName << "' value at fieldIndex " << fieldIndex << std::endl;
        } else {
            llvm::Type* attrLLVMType = context.typeSystem.getLLVMTypeForHulkType(attr.TypeName);
            std::cerr << "[DEBUG] No initializer for attribute '" << attrName << "', using default value for type '" << attr.TypeName << "' (LLVM type: " << attrLLVMType << ")\n";
            llvm::Value* defaultValue = llvm::Constant::getNullValue(attrLLVMType);
            llvm::Value* fieldPtr = context.builder.CreateStructGEP(typeDef.llvmStructType, instancePtr, fieldIndex);
            context.builder.CreateStore(defaultValue, fieldPtr);
            std::cerr << "[DEBUG] Stored default value for attribute '" << attrName << "' at fieldIndex " << fieldIndex << std::endl;
        }
        fieldIndex++;
    }
    
    // Clean up the temporary scope
    context.popVarScope();
    
    // Push the instance pointer onto the value stack so it can be stored in the local scope
    context.valueStack.push_back(instancePtr);
    
    std::cerr << "[DEBUG] Finished NewInstanceNode for type: '" << typeDef.name << "'" << std::endl;
}

void LLVMGenerator::visit(MethodCallNode& node) {
    std::cout << "🔍 MethodCall: " << node.instanceName << "." << node.methodName << std::endl;

    // Get the instance value from the stack or lookup
    llvm::Value* instancePtr = nullptr;
    
    if (node.instanceName == "self") {
        // Get the current instance from the value stack or context
        if (!context.valueStack.empty()) {
            instancePtr = context.valueStack.back();
        } else {
            throw std::runtime_error("❌ No instance available for 'self' call at line " + std::to_string(node.line()));
        }
    } else {
        // Look up the instance variable
        instancePtr = context.lookupLocal(node.instanceName);
        if (!instancePtr) {
            throw std::runtime_error("❌ Instance '" + node.instanceName + "' not found at line " + std::to_string(node.line()));
        }
    }

    // Get the type information
    std::string typeName = context.typeSystem.getInstanceType(node.instanceName);
    if (typeName.empty()) {
        throw std::runtime_error("❌ Type information not found for instance at line " + std::to_string(node.line()));
    }

    auto& typeDef = context.typeSystem.getTypeDefinition(typeName);
    if (!typeDef.llvmStructType || !typeDef.vtableGlobal) {
        throw std::runtime_error("❌ LLVM type or vtable not available for type '" + typeName + "' at line " + std::to_string(node.line()));
    }

    // Find the method in the type hierarchy
    TypeMethod* method = context.typeSystem.findMethod(typeName, node.methodName);
    if (!method || !method->llvmFunction) {
        throw std::runtime_error("❌ Method '" + node.methodName + "' not found in type '" + typeName + "' at line " + std::to_string(node.line()));
    }

    // Prepare arguments for the method call
    std::vector<llvm::Value*> args;
    args.push_back(instancePtr); // 'this' pointer as first argument

    // Process method arguments
    for (ASTNode* arg : node.args) {
        arg->accept(*this);
        if (context.valueStack.empty()) {
            throw std::runtime_error("❌ Failed to evaluate method argument at line " + std::to_string(node.line()));
        }
        args.push_back(context.valueStack.back());
        context.valueStack.pop_back();
    }

    // Call the method function
    llvm::Type* retType = method->llvmFunction->getReturnType();
    if (retType->isVoidTy()) {
        context.builder.CreateCall(method->llvmFunction, args);
        // Do not push to value stack for void
    } else {
        llvm::Value* result = context.builder.CreateCall(method->llvmFunction, args, "basecall_result");
        context.valueStack.push_back(result);
    }

    std::cout << "✅ Method call processed: " << node.methodName << std::endl;
}

void LLVMGenerator::visit(SelfCallNode& node) {
    std::cout << "🔍 SelfCall: " << node.varName << " - Stack size before: " << context.valueStack.size() << std::endl;

    std::string currentType = context.typeSystem.getCurrentType();
    if (currentType.empty()) {
        throw std::runtime_error("❌ 'self' access outside of type context at line " + std::to_string(node.line()));
    }
    if (context.valueStack.empty()) {
        throw std::runtime_error("❌ No instance available for 'self' access at line " + std::to_string(node.line()));
    }
    llvm::Value* instancePtr = context.valueStack.back();
    auto& typeDef = context.typeSystem.getTypeDefinition(currentType);
    if (!typeDef.llvmStructType) {
        throw std::runtime_error("❌ LLVM struct type not available for type '" + currentType + "' at line " + std::to_string(node.line()));
    }
    // Use new recursive attribute pointer logic
    llvm::Value* fieldPtr = getAttributePtr(instancePtr, typeDef, node.varName);
    auto* attr = context.typeSystem.findAttribute(currentType, node.varName);
    if (!attr) {
        throw std::runtime_error("❌ Attribute '" + node.varName + "' not found in type hierarchy at line " + std::to_string(node.line()));
    }
    llvm::Value* fieldValue = context.builder.CreateLoad(attr->llvmType, fieldPtr, node.varName + "_value");
            context.valueStack.push_back(fieldValue);
            std::cout << "  📤 Self field accessed: " << node.varName << " - Final stack size: " << context.valueStack.size() << std::endl;
            std::cout << "✅ Self access processed" << std::endl;
}

void LLVMGenerator::visit(BaseCallNode& node) {
    std::cout << "🔍 BaseCall - Stack size before: " << context.valueStack.size() << std::endl;
    PlaceholderEntry currentPlaceholder = context.typeSystem.getCurrentPlaceholder();
    std::string name = currentPlaceholder.name;
    std::string elemType = currentPlaceholder.type;
    if (name.empty() || elemType.empty()) {
        throw std::runtime_error("❌ Invalid placeholder for base call at line " + std::to_string(node.line()));
    }
    std::string currentType = context.typeSystem.getCurrentType();
    if (currentType.empty()) {
        throw std::runtime_error("❌ Base call outside of type context at line " + std::to_string(node.line()));
    }
    std::optional<std::string> parentType = context.typeSystem.getParentType(currentType);
    if (!parentType) {
        throw std::runtime_error("❌ No parent type found for type '" + currentType + "' at line " + std::to_string(node.line()));
    }
    if (elemType == "var") {
        if (context.valueStack.empty()) {
            throw std::runtime_error("❌ No instance available for base call at line " + std::to_string(node.line()));
        }
        llvm::Value* instancePtr = context.valueStack.back();
        auto& currentTypeDef = context.typeSystem.getTypeDefinition(currentType);
        auto& parentTypeDef = context.typeSystem.getTypeDefinition(*parentType);
        // Load parent pointer (field 2)
        llvm::Value* parentPtr = context.builder.CreateStructGEP(currentTypeDef.llvmStructType, instancePtr, 2);
        llvm::Value* loadedParent = context.builder.CreateLoad(
            llvm::PointerType::get(parentTypeDef.llvmStructType, 0), parentPtr, "parent_loaded");
        // Use getAttributePtr recursively on parent
        llvm::Value* fieldPtr = getAttributePtr(loadedParent, parentTypeDef, name);
        auto* attr = context.typeSystem.findAttribute(*parentType, name);
        if (!attr) {
            throw std::runtime_error("❌ Attribute '" + name + "' not found in parent type at line " + std::to_string(node.line()));
        }
        llvm::Value* fieldValue = context.builder.CreateLoad(attr->llvmType, fieldPtr, name + "_base_value");
        context.valueStack.push_back(fieldValue);
        std::cout << "  📤 Base variable accessed: " << name << " - Final stack size: " << context.valueStack.size() << std::endl;
        std::cout << "✅ Base variable access processed" << std::endl;
        return;
    }
    // ... existing method call logic ...
}

// Helper for recursive attribute pointer lookup
llvm::Value* LLVMGenerator::getAttributePtr(llvm::Value* instancePtr, TypeDefinition& typeDef, const std::string& attrName) {
    int fieldIndex = 3; // typeId=0, vtable=1, parent pointer=2, then attributes
    int i = 0;
    for (const auto& [name, attr] : typeDef.attributes) {
        if (name == attrName) {
            fieldIndex += i;
            return context.builder.CreateStructGEP(typeDef.llvmStructType, instancePtr, fieldIndex, attrName + "_ptr");
        }
        ++i;
    }
    if (typeDef.parentDef != nullptr) {
        std::cerr << "[DEBUG] getAttributePtr: type '" << typeDef.name << "' has parent '" << typeDef.parentDef->name << "'\n";
        if (!typeDef.parentDef->llvmStructType) {
            std::cerr << "[ERROR] getAttributePtr: parentDef->llvmStructType is nullptr for parent: '" << typeDef.parentDef->name << "'\n";
        }
        // Load parent pointer (field 2)
        llvm::Value* parentPtr = context.builder.CreateStructGEP(typeDef.llvmStructType, instancePtr, 2);
        llvm::Value* loadedParent = context.builder.CreateLoad(
            llvm::PointerType::get(typeDef.parentDef->llvmStructType, 0), parentPtr, "parent_loaded");
        return getAttributePtr(loadedParent, *typeDef.parentDef, attrName);
        } else {
        std::cerr << "[DEBUG] getAttributePtr: type '" << typeDef.name << "' has no parent\n";
    }
    throw std::runtime_error("Attribute '" + attrName + "' not found in type hierarchy");
}

// Add this method to create a main function and set up the builder
void LLVMGenerator::setupMainFunction() {
    // Create main function type: int main() { ... }
    llvm::FunctionType* mainType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(context.context),  // return type: int
        false  // not vararg
    );
    
    // Create the main function
    llvm::Function* mainFunc = llvm::Function::Create(
        mainType,
        llvm::Function::ExternalLinkage,
        "main",
        &context.module
    );
    
    // Create the entry basic block
    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(
        context.context,
        "entry",
        mainFunc
    );
    
    // Set the builder's insertion point to the entry block
    context.builder.SetInsertPoint(entryBlock);
    
    std::cerr << "[DEBUG] Created main function and set insertion point to entry block" << std::endl;
}

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
    std::cout << "🔍 LetNode - Stack size before: " << context.valueStack.size() << std::endl;
    
    // 1. Initialize new variable scope with inheritance
    context.pushVarScope(true);  // true for inheritance

    // 2. Process each declaration
    for (const LetDeclaration& decl : *node.declarations) {
        // Push the variable name onto the placeholder stack before processing its value
        context.typeSystem.pushPlaceholder(decl.name);
        std::cout << "  📝 Added placeholder: " << decl.name << std::endl;

        // Process the initializer
        decl.initializer->accept(*this);
        llvm::Value* initValue = context.valueStack.back();
        context.valueStack.pop_back();

        // Add the variable to the current scope
        context.addLocal(decl.name, initValue);
        std::cout << "  📝 Added variable '" << decl.name << "' to scope" << std::endl;

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
        throw std::runtime_error("❌ Left-hand side of assignment must be an identifier at line " + 
                               std::to_string(node.line()));
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
            context.typeSystem.addAttribute(attr.name, node.name, attr.initializer);
            std::cout << "  📝 Added attribute: " << attr.name << std::endl;
        }
    }

    // Process methods
    if (node.body->methods) {
        for (const auto& method : *node.body->methods) {
            context.typeSystem.addMethod(node.name, method.name, method.params, method.body, method.returnType);
            std::cout << "  📝 Added method: " << method.name << std::endl;
        }
    }

    std::cout << "✅ Type " << node.name << " processed" << std::endl;
}

void LLVMGenerator::visit(NewInstanceNode& node) {
    std::cout << "🔍 NewInstance: " << node.typeName << std::endl;

    // Check if we're inside a let declaration
    std::string varName = context.typeSystem.getCurrentPlaceholder();
    if (!varName.empty()) {
        std::cout << "  📝 Using placeholder variable: " << varName << std::endl;
    }

    // 1. Initialize instance variables map and set current type
    std::map<std::string, llvm::Value*> instanceVars;
    std::string currType = node.typeName;
    context.typeSystem.setCurrentType(currType);

    // 2. Start a new variable scope without inheritance
    context.pushVarScope(false);

    // Process constructor arguments first
    std::vector<llvm::Value*> args;
    for (ASTNode* arg : node.args) {
        arg->accept(*this);
        args.push_back(context.valueStack.back());
        context.valueStack.pop_back();
    }

    // Process type hierarchy
    while (!currType.empty()) {
        std::cout << "  🔄 Processing type: " << currType << std::endl;
        
        // Get type information
        const auto& typeConst = context.typeSystem.getConstructorParams(currType);
        const auto& fatherArgs = context.typeSystem.getBaseArgs(currType);
        const auto& attrType = context.typeSystem.getAttributes(currType);
        const auto& father = context.typeSystem.getParentType(currType);

        // Process constructor parameters
        if (!typeConst.empty()) {
            for (size_t i = 0; i < typeConst.size() && i < args.size(); ++i) {
                context.addLocal(typeConst[i], args[i]);
                std::cout << "    📝 Added constructor param: " << typeConst[i] << std::endl;
            }
            // Remove processed args
            args.erase(args.begin(), args.begin() + std::min(typeConst.size(), args.size()));
        }

        // Process parent constructor parameters
        if (father) {
            const auto& fatherConst = context.typeSystem.getConstructorParams(*father);
            
            // Process base args first
            if (!fatherArgs.empty()) {
                for (size_t i = 0; i < fatherArgs.size() && i < fatherConst.size(); ++i) {
                    fatherArgs[i]->accept(*this);
                    context.addLocal(fatherConst[i], context.valueStack.back());
                    context.valueStack.pop_back();
                    std::cout << "    📝 Added base arg: " << fatherConst[i] << std::endl;
                }
            }

            // Process remaining args with remaining parent constructor params
            size_t startIdx = fatherArgs.size();
            for (size_t i = 0; i < fatherConst.size() - startIdx && i < args.size(); ++i) {
                context.addLocal(fatherConst[startIdx + i], args[i]);
                std::cout << "    📝 Added remaining parent param: " << fatherConst[startIdx + i] << std::endl;
            }
            // Remove processed args
            args.erase(args.begin(), args.begin() + std::min(fatherConst.size() - startIdx, args.size()));
        }

        // Process attributes
        for (const auto& [attrName, attr] : attrType) {
            if (attr.initializer) {
                attr.initializer->accept(*this);
                instanceVars[attrName] = context.valueStack.back();
                context.valueStack.pop_back();
                std::cout << "    📝 Added attribute: " << attrName << std::endl;
            }
        }

        // Move to parent type
        if (father) {
            currType = *father;
            context.typeSystem.setCurrentType(currType);
        }
    }

    // Clean up
    context.popVarScope();
    context.typeSystem.setCurrentType("");

    // Create the instance with its variables
    context.typeSystem.createInstance(varName, node.typeName, instanceVars);

    std::cout << "✅ Instance created: " << varName << std::endl;
}

void LLVMGenerator::visit(MethodCallNode& node) {
    std::cout << "🔍 MethodCall: " << node.instanceName << "." << node.methodName << std::endl;

    // 1. Get instance type and set currentType
    std::string typeName = context.typeSystem.getInstanceType(node.instanceName);
    if (typeName.empty()) {
        throw std::runtime_error("Instance '" + node.instanceName + "' not found");
    }
    context.typeSystem.setCurrentType(typeName);

    // 2. Push instance variables onto the stack
    context.typeSystem.pushCurrentInstanceVars(context.typeSystem.getInstanceVars(node.instanceName));

    // 3. Create new varScope without inheritance
    context.pushVarScope(false);

    // 4. Set method name in placeholderStack
    context.typeSystem.pushPlaceholder(node.methodName);

    // 5. Find method in type hierarchy
    TypeMethod* method = nullptr;
    std::string currType = typeName;
    while (!currType.empty() && !method) {
        method = context.typeSystem.findMethod(currType, node.methodName);
        if (!method) {
            currType = context.typeSystem.getParentType(currType).value_or("");
        }
    }

    if (!method) {
        throw std::runtime_error("Method '" + node.methodName + "' not found in type hierarchy starting from '" + typeName + "'");
    }

    // 6. Process method arguments and parameters
    std::vector<llvm::Value*> args;
    for (ASTNode* arg : node.args) {
        arg->accept(*this);
        args.push_back(context.valueStack.back());
        context.valueStack.pop_back();
    }

    // Associate parameters with argument values
    if (method->params) {
        for (size_t i = 0; i < method->params->size() && i < args.size(); ++i) {
            context.addLocal((*method->params)[i].name, args[i]);
            std::cout << "  📝 Bound param " << (*method->params)[i].name << std::endl;
        }
    }

    // 7. Evaluate method body
    method->body->accept(*this);

    // 8. Clean up
    context.typeSystem.setCurrentType("");
    context.typeSystem.popPlaceholder();
    context.popVarScope();
    context.typeSystem.popCurrentInstanceVars();

    std::cout << "✅ Method call processed" << std::endl;
}

void LLVMGenerator::visit(SelfCallNode& node) {
    std::cout << "🔍 SelfCall: " << node.varName << std::endl;

    // Handle self variables
    llvm::Value* val = context.getCurrentInstanceVar(node.varName);

    if (!val) {
        throw std::runtime_error("❌ Undefined variable '" + node.varName +
                                 "' at line " + std::to_string(node.line()));
    }

    context.valueStack.push_back(val);

    std::cout << "✅ Self access processed" << std::endl;
}

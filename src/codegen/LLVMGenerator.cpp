#include "LLVMGenerator.hpp"
#include "CodeGenContext.hpp"
#include "../ast/AST.hpp"
#include <stdexcept>
#include <iostream>
#include <string>
#include <cstring>

LLVMGenerator::LLVMGenerator(CodeGenContext& ctx)
    : context(ctx) {}

std::string processRawString(const std::string& raw) {
    std::string processed;
    bool escape = false;
    size_t start = (raw.front() == '"') ? 1 : 0;
    size_t end = (raw.back() == '"') ? raw.size() -1 : raw.size(); // Adjusted

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

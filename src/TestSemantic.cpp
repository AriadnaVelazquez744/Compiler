// TestSemantic.cpp
#include <iostream>
#include "AST.h"
#include "SemanticAnalyzer.h"

// Función para probar un caso básico
void testBasicArithmetic() {
    // AST para la expresión: (5 + 3) * 2
    ASTNode* expr = new BinaryOpNode(
        "*",
        new BinaryOpNode(
            "+",
            new LiteralNode("5", "number", 1),
            new LiteralNode("3", "number", 1),
            1
        ),
        new LiteralNode("\"error\"", "string", 1), // ¡Tipo incorrecto!
        1
    );

    SemanticAnalyzer analyzer;
    expr->accept(analyzer); // Ejecutar análisis semántico

    if (analyzer.getErrors().empty()) {
        std::cout << "✅ Test básico PASÓ: Expresión aritmética válida\n";
        std::cout << "Tipo inferido: " << expr->type() << "\n"; // Debe ser "number"
    } else {
        std::cerr << "❌ Test básico FALLÓ:\n";
        for (auto& error : analyzer.getErrors()) {
            std::cerr << "Línea " << error.line << ": " << error.message << "\n";
        }
    }

    delete expr; // Liberar memoria
}

int main() {
    testBasicArithmetic();
    return 0;
}
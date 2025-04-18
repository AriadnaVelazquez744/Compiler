#include <iostream>
#include "AST.hpp"
#include "SemanticAnalyzer.hpp"

void testFunctionSemantics() {
    // Test 1: Parámetros duplicados
    auto params = std::vector<Parameter>{{"x", "number"}, {"x", "number"}};
    ASTNode* func = new FunctionDeclarationNode(
        "suma",
        "number",
        params,
        new BinaryOpNode("+", new IdentifierNode("x", 1), new IdentifierNode("x", 1), 1),
        false,
        1
    );

    SemanticAnalyzer analyzer;
    analyzer.analyze(func);

    if (!analyzer.getErrors().empty()) {
        std::cout << "✅ Test 1 PASÓ: Detectó parámetros duplicados\n";
    } else {
        std::cout << "❌ Test 1 FALLÓ\n";
    }

    delete func;
}

void testVariableShadowing() {
    // Test 2: Shadowing válido
    LetDeclaration outerDecl{"a", "", new LiteralNode("5", "number", 1)};
    LetDeclaration innerDecl{"a", "", new LiteralNode("\"Hulk\"", "string", 2)};
    
    ASTNode* innerLet = new LetNode(
        {innerDecl}, 
        new FunctionCallNode("print", {new IdentifierNode("a", 2)}, 2),
        2
    );
    
    ASTNode* outerLet = new LetNode(
        {outerDecl}, 
        innerLet, 
        1
    );

    SemanticAnalyzer analyzer;
    outerLet->accept(analyzer);

    if (analyzer.getErrors().empty()) {
        std::cout << "✅ Test 2 PASÓ: Shadowing válido\n";
    } else {
        std::cout << "❌ Test 2 FALLÓ\n";
    }

    delete outerLet;
}

void testInvalidAssignment() {
    // Test 3: Asignación inválida
    LetDeclaration decl{"a", "", new LiteralNode("5", "number", 1)};
    ASTNode* body = new AssignmentNode("a", new LiteralNode("\"Hulk\"", "string", 1), 1);
    ASTNode* letExpr = new LetNode({decl}, body, 1);

    SemanticAnalyzer analyzer;
    letExpr->accept(analyzer);

    if (analyzer.getErrors().size() == 1) {
        std::cout << "✅ Test 3 PASÓ: Detectó asignación inválida\n";
    } else {
        std::cout << "❌ Test 3 FALLÓ\n";
    }

    delete letExpr;
}

int main() {
    testFunctionSemantics();
    testVariableShadowing();
    testInvalidAssignment();
    
    return 0;
}
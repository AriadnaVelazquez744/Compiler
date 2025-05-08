#include <iostream>
#include "../src/ast/AST.hpp"
#include "../src/semantic/SemanticAnalyzer.hpp"

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

void testValidConditional() {
    // if (true) 42 else 10
    IfBranch mainBranch{
        new LiteralNode("true", "boolean", 1),
        new LiteralNode("42", "number", 1)
    };
    
    ASTNode* elseBody = new LiteralNode("10", "number", 1);
    ASTNode* ifExpr = new IfNode({mainBranch}, elseBody, 1);

    SemanticAnalyzer analyzer;
    ifExpr->accept(analyzer);

    if (analyzer.getErrors().empty() && ifExpr->type() == "number") {
        std::cout << "✅ Test 1 PASÓ: If válido\n";
    } else {
        std::cout << "❌ Test 1 FALLÓ\n";
    }
    
    delete ifExpr;
}

void testInvalidConditionType() {
    // if (5) "a" else "b"
    IfBranch mainBranch{
        new LiteralNode("5", "number", 1),
        new LiteralNode("\"a\"", "string", 1)
    };
    
    ASTNode* elseBody = new LiteralNode("\"b\"", "string", 1);
    ASTNode* ifExpr = new IfNode({mainBranch}, elseBody, 1);

    SemanticAnalyzer analyzer;
    ifExpr->accept(analyzer);

    if (!analyzer.getErrors().empty()) {
        std::cout << "✅ Test 2 PASÓ: Detectó condición no booleana\n";
    } else {
        std::cout << "❌ Test 2 FALLÓ\n";
    }
    
    delete ifExpr;
}

void testTypeMismatch() {
    // if (true) 42 else "Hulk"
    IfBranch mainBranch{
        new LiteralNode("true", "boolean", 1),
        new LiteralNode("42", "number", 1)
    };
    
    ASTNode* elseBody = new LiteralNode("\"Hulk\"", "string", 1);
    ASTNode* ifExpr = new IfNode({mainBranch}, elseBody, 1);

    SemanticAnalyzer analyzer;
    ifExpr->accept(analyzer);

    if (!analyzer.getErrors().empty()) {
        std::cout << "✅ Test 3 PASÓ: Detectó tipos incompatibles\n";
    } else {
        std::cout << "❌ Test 3 FALLÓ\n";
    }
    
    delete ifExpr;
}

void testValidWhileLoop() {
    // let a = 10 in while (a >= 0) { ... }
    LetDeclaration decl{"a", "", new LiteralNode("10", "number", 1)};
    
    // Crear condición: a >= 0
    ASTNode* condition = new BinaryOpNode(
        ">=",
        new IdentifierNode("a", 1),
        new LiteralNode("0", "number", 1),
        1
    );
    
    // Crear cuerpo del while
    ASTNode* body = new BlockNode({
        new FunctionCallNode("print", {new IdentifierNode("a", 1)}, 1),
        new AssignmentNode("a", 
            new BinaryOpNode("-", new IdentifierNode("a", 1), new LiteralNode("1", "number", 1), 1),
            1
        )
    }, 1);
    
    // Crear nodos del AST
    ASTNode* whileLoop = new WhileNode(condition, body, 1);
    ASTNode* letExpr = new LetNode({decl}, whileLoop, 1); // Ámbito para 'a'

    SemanticAnalyzer analyzer;
    letExpr->accept(analyzer); // Analizar desde el let

    if (analyzer.getErrors().empty()) {
        std::cout << "✅ Test 1 PASÓ: While válido\n";
    } else {
        std::cout << "❌ Test 1 FALLÓ\n";
        for (auto& error : analyzer.getErrors()) {
            std::cout << "Error: " << error.message << " (línea " << error.line << ")\n";
        }
    }

    delete letExpr;
}

void testInvalidForLoop() {
    // for (x in 42) print(x); (42 no es iterable)
    ASTNode* iterable = new LiteralNode("42", "number", 1);
    ASTNode* forLoop = new ForNode("x", iterable, new FunctionCallNode("print", {new IdentifierNode("x", 1)}, 1), 1);

    SemanticAnalyzer analyzer;
    forLoop->accept(analyzer);

    if (!analyzer.getErrors().empty()) {
        std::cout << "✅ Test 2 PASÓ: Detectó iterable inválido\n";
    } else {
        std::cout << "❌ Test 2 FALLÓ\n";
    }

    delete forLoop;
}

void testValidTypeInheritance() {
    // type PolarPoint(phi, rho) inherits Point(rho * sin(phi), rho * cos(phi)) {}
    ASTNode* parentArg1 = new BinaryOpNode("*", 
        new IdentifierNode("rho", 1), 
        new FunctionCallNode("sin", {new IdentifierNode("phi", 1)}, 1), 
        1
    );
    ASTNode* parentArg2 = new BinaryOpNode("*", 
        new IdentifierNode("rho", 1), 
        new FunctionCallNode("cos", {new IdentifierNode("phi", 1)}, 1), 
        1
    );
    TypeDeclarationNode* type = new TypeDeclarationNode(
        "PolarPoint",
        {"phi", "rho"},
        {}, // Atributos (simplificado)
        {}, // Métodos (simplificado)
        "Point",
        {parentArg1, parentArg2},
        1
    );

    SemanticAnalyzer analyzer;
    type->accept(analyzer);

    if (analyzer.getErrors().empty()) {
        std::cout << "✅ Test 1 PASÓ: Herencia válida\n";
    } else {
        std::cout << "❌ Test 1 FALLÓ\n";
    }
}

int main() {
    testFunctionSemantics();
    testVariableShadowing();
    testInvalidAssignment();
    testValidConditional();
    testInvalidConditionType();
    testTypeMismatch();
    testValidWhileLoop();
    testInvalidForLoop();
    testValidTypeInheritance();
    return 0;
}
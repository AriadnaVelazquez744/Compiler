#include <cstdio>
#include <iostream>
#include <vector>
#include "ast/AST.hpp"
#include "semantic/SemanticAnalyzer.hpp"
#include "codegen/CodeGenContext.hpp"

extern int yyparse();
extern FILE *yyin;
extern std::vector<ASTNode*> root; // Nodo raíz del AST generado por el parser

bool is_valid_ast(const std::vector<ASTNode*>& nodes) {
    // Verificar que el vector no esté vacío y que todos los nodos sean válidos
    if (nodes.empty()) {
        std::cerr << "AST vacío: ningún nodo generado" << std::endl;
        return false;
    }
    
    for (auto node : nodes) {
        if (!node) {
            std::cerr << "AST contiene nodos nulos" << std::endl;
            return false;
        }
    }
    return true;
}

void delete_ast(std::vector<ASTNode*>& nodes) {
    for (auto node : nodes) {
        delete node; // Liberar cada nodo individualmente
    }
    nodes.clear(); // Limpiar el vector
}

int main(int argc, char **argv) {
    const char* filename = "script.hulk";  // Default file
    
    if (argc >= 2) {
        filename = argv[1];  // Use provided filename
    }

    FILE *input_file = fopen(filename, "r");
    if (!input_file) {
        perror("Error opening file");
        return 1;
    }

    yyin = input_file;
    //yyparse();
    if (yyparse() != 0) { // Realizar el análisis sintáctico
        std::cerr << "Error: Falló el análisis sintáctico." << std::endl;
        fclose(input_file);
        return 1;
    }
    fclose(input_file);

    if (!is_valid_ast(root)) {
        std::cerr << "Error: No se generó un AST válido." << std::endl;
        return 1;
    }


    std::cout << "AST terminado." << std::endl;

    for (auto node : root) {
        std::cout << "Tipo de nodo raíz: " << node->type() 
                << " | Línea: " << node->line() << "\n";
    }

    // Crear el analizador semántico
    SemanticAnalyzer semanticAnalyzer;

    std::cout << "Instancia anlizador creada." << std::endl;

    // Realizar el análisis semántico
    semanticAnalyzer.analyze(root);

    std::cout << "Análisis semántico completado exitosamente." << std::endl;

    CodeGenContext codegen;
        
    std::cout << "Instancia generador definida." << std::endl;

    try {
        codegen.generateCode(root);
    } catch (const std::exception& e) {
        std::cerr << "Error during code generation: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Generación de código completada." << std::endl;

    codegen.dumpIR("hulk-low-code.ll");

    std::cout << "Volcando IR en hulk-low-code.ll." << std::endl;

    // Liberar memoria del AST
    delete_ast(root);

    return 0;
}
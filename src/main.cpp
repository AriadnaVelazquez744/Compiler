#include <cstdio>
#include <iostream>
#include <vector>
#include "ast/AST.hpp"
#include "semantic/SemanticAnalyzer.hpp"
#include "codegen/CodeGenContext.hpp"

extern int yyparse();
extern FILE *yyin;
extern ASTNode* root; // Nodo raíz del AST generado por el parser

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

    if (!root) {
        std::cerr << "Error: No se generó un AST válido." << std::endl;
        return 1;
    }


    std::cout << "AST terminado." << std::endl;

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

    codegen.dumpIR("output.ll");

    std::cout << "Volcando IR en output.ll." << std::endl;

    // Liberar memoria del AST
    delete root;

    return 0;
}
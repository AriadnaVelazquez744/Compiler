#include <cstdio>
#include <iostream>
#include <vector>
#include "ast/AST.hpp"
#include "semantic/SemanticAnalyzer.hpp"

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
    // SymbolTable symbolTable;
    // std::vector<std::string> errors;
    SemanticAnalyzer semanticAnalyzer;

    std::cout << "Instancia anlizador creada." << std::endl;

    // Realizar el análisis semántico
    semanticAnalyzer.analyze(root);

    // Manejar errores semánticos
    // if (!errors.empty()) {
    //     std::cerr << "Errores semánticos encontrados:" << std::endl;
    //     for (const auto& error : errors) {
    //         std::cerr << "  - " << error << std::endl;
    //     }
    //     return 1;
    // }

    std::cout << "Análisis semántico completado exitosamente." << std::endl;

    // Liberar memoria del AST
    delete root;

    return 0;
}
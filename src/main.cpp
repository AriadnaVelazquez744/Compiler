#include <iostream>
#include <cstdio> // Para FILE*
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"

extern FILE* yyin; // Archivo de entrada para el lexer
extern int yyparse(); // Función principal del parser

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo.hulk>\n";
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        std::cerr << "No se pudo abrir el archivo: " << argv[1] << "\n";
        return 1;
    }

    yyin = file; // Asigna el archivo al lexer

    std::cout << "Iniciando análisis...\n";

    // Llama al parser (el parser invoca al lexer automáticamente)
    if (yyparse() == 0) {
        std::cout << "Análisis completado: Expresión válida.\n";
    } else {
        std::cerr << "Error en el análisis sintáctico.\n";
    }

    fclose(file); // Cierra el archivo
    return 0;
}

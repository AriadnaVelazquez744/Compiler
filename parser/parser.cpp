#include "parser.hpp"
#include <iostream>

namespace HULK {
    extern int yyparse(); // Declaración de la función generada por Bison

    int Parser::parse() {
        std::cout << "Iniciando análisis sintáctico..." << std::endl;
        return yyparse(); // Llama a Bison para iniciar el parsing
    }
}

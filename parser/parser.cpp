#include "parser.hpp"

void runParser() {
    std::cout << "Iniciando análisis sintáctico...\n";
    yyparse();
}

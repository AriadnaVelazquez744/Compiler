#include "parser.h"

void runParser() {
    std::cout << "Iniciando análisis sintáctico...\n";
    yyparse();
}

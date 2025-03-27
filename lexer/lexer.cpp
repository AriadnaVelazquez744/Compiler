#include "lexer.h"
#include "../parser/parser.h"  // Necesario porque Flex usa Bison

void runLexer() {
    std::cout << "Ejecutando lexer...\n";
    int token;
    while ((token = yylex())) {
        std::cout << "Token: " << token << std::endl;
    }
}

#ifndef LEXER_H
#define LEXER_H

#include <cstdio> // Para FILE*
#include "../parser/parser.hpp"

namespace HULK {
    int yylex(); // Declaración de la función generada por Flex

    // Declaración de la variable global yyin utilizada por Flex
    extern FILE* yyin;
}

#endif // LEXER_H
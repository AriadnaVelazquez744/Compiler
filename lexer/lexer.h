#ifndef LEXER_H
#define LEXER_H

#include <iostream>

extern int yylex();  // Función de Flex
extern FILE* yyin;   // Archivo de entrada

void runLexer();

#endif // LEXER_H

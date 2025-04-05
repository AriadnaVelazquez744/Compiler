%{
#include "../include/tokens.hpp"
#include <stdio.h>

extern int yylex();
extern int yylineno;
void yyerror(const char *msg);
%}

%define api.token.prefix {TOK_}
%token NUMBER

%%

program:
    /* vacío */
    | program expression
;

expression:
    NUMBER { printf("Número reconocido: %d\n", $1); }
;

%%

void yyerror(const char *msg) {
    fprintf(stderr, "Error en línea %d: %s\n", yylineno, msg);
}


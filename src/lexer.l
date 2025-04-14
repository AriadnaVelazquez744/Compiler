%{
#include "../build/parser.tab.hpp"     

// Variables para seguimiento de columnas
int yycolumn = 1;

// Macro que se ejecuta ANTES de cada token
#define YY_USER_ACTION \
    yylloc.first_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_line = yylineno; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;  // Actualizar después de registrar la posición
%}

%option noyywrap         
%option yylineno   
%option nounput       

%%

-?[0-9]+(\.[0-9]+)?     { 
                            yylval.num = atof(yytext); 
                            printf("Token: %s → Línea %d, Columna %d\n", yytext, yylineno, yylloc.first_column);
                            return NUMBER; 
                        }

[ \t]+                  { /* Whitespace handled by YY_USER_ACTION */ }  // Ignorar espacios

\n                      { yycolumn = 1; }        // Resetear columna al salto de línea

.                       { 
                            fprintf(stderr, "Error léxico: '%s' en %d:%d\n", yytext, yylineno, yylloc.first_column); 
                            //yycolumn++; 
                        }

%%
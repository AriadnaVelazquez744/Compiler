%{
#include "../build/parser.tab.hpp"     
extern YYLTYPE yylloc;     // Para ubicaciones

// Variables para manejar columnas
int yycolumn = 1;
#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;
%}

%option noyywrap         
%option yylineno   
%option nounput       

%%

-?[0-9]+(\.[0-9]+)?  { 
                            yylval.num = atof(yytext); 
                            return NUMBER; 
                        }

[ \t]                   { yycolumn += yyleng; }  // Ignorar espacios

\n                      { yycolumn = 1; }        // Resetear columna al salto de línea

.                       { 
                            fprintf(stderr, "Error léxico: '%s' en línea %d, columna %d\n", yytext, yylineno, yycolumn); 
                            yycolumn++; 
                        }

%%
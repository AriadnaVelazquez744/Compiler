%{
#include "../build/parser.tab.hpp"     
extern YYSTYPE yylval;   // Variable para valores semánticos
extern int yylineno;  // Para seguimiento de líneas
%}

%option noyywrap         
%option yylineno   
%option nounput       

%%

-?[0-9]+    { 
                        yylval = atoi(yytext); 
                        return TOK_NUMBER; 
                    }

[ \t\n]             ;  // Ignorar espacios y saltos

.                   { fprintf(stderr, "Carácter inválido: %s\n", yytext); }

%%
%{
#include "../build/parser.tab.hpp"     
#include <string>

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

[0-9]+(\.[0-9]+)?     { 
                            yylval.num = atof(yytext); 
                            printf("Token: %s → Línea %d, Columna %d\n", yytext, yylineno, yylloc.first_column);
                            return NUMBER; 
                        }
\"([^\"]*)\"            {
                            std::string raw = yytext;
                            yylval.str = new std::string(raw.substr(1, raw.size() - 2));
                            printf("String: %s  → Línea %d, Columna %d\n", yytext, yylineno, yylloc.first_column);
                            return STRING;
                        }
"True"                  {
                            yylval.boolean = true;
                            printf("Boolean: %s  → Línea %d, Columna %d\n", yytext, yylineno, yylloc.first_column);
                            return BOOL;
                        }
"False"                 {
                            yylval.boolean = false;
                            printf("Boolean: %s  → Línea %d, Columna %d\n", yytext, yylineno, yylloc.first_column);
                            return BOOL;
                        }
"Null"                  { 
                            printf("Null value → Línea %d, Columna %d\n", yylineno, yylloc.first_column);
                            return NULL_VAL; 
                        }


[+]                     { printf(" + \n"); return ADD; }
[-]                     { printf(" - \n"); return SUB; }
[*]                     { printf(" * \n"); return MUL; }
[/]                     { printf(" / \n"); return DIV; }
[%]                     { printf(" mod \n"); return MOD; }
[\^]                     { printf(" ^ \n"); return POW; }


[<]                     { printf(" < \n"); return LT; }
[>]                     { printf(" > \n"); return GT; }
"<="                    { printf(" <= \n"); return LE; }
">="                    { printf(" >= \n"); return GE; }
"=="                    { printf(" == \n"); return EQ; }
"!="                    { printf(" != \n"); return NE; }


"&"                     { printf(" & \n"); return AND; }
"!"                     { printf(" ! \n"); return NOT; }
[|]                     { printf(" | \n"); return OR; }


"@"                     { printf(" @ \n"); return CONCAT; }
"@@"                    { printf(" @@ \n"); return CONCAT_SPACE; }


"sin"                   { printf(" sin \n"); return SIN; }
"cos"                   { printf(" cos \n"); return COS; }
"max"                   { printf(" max \n"); return MAX; }
"min"                   { printf(" min \n"); return MIN; }
"sqrt"                  { printf(" sqrt \n"); return SQRT; }
"exp"                   { printf(" exp \n"); return EXP; }
"log"                   { printf(" log \n"); return LOG; }
"rand"                  { printf(" rand \n"); return RANDOM; }


"print"                 { printf("print\n"); return PRINT; }
"read"                  { printf("read\n"); return READ; } 
"parse"                 { printf("parse\n"); return PARSE; }


"("                     { printf(" ( \n"); return '('; }
")"                     { printf(" ) \n"); return ')'; }
";"                     { 
                            printf("';' symbol  → Línea %d, Columna %d\n", yylineno, yylloc.first_column);
                            return ';'; 
                        }


[ \t]+                  { /* Whitespace handled by YY_USER_ACTION */ }  // Ignorar espacios
\n                      { yycolumn = 1; }        // Resetear columna al salto de línea
.                       { 
                            fprintf(stderr, "Error léxico: '%s' en %d:%d\n", yytext, yylineno, yylloc.first_column); 
                        }

%%
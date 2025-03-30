%option c++ noyywrap

%{
#include "lexer.hpp"
#include <cstdlib>
#include <iostream>
#include <string>      // Para std::string
#include <stdexcept>   // Para excepciones
%}

/* ========== Definición de Tokens ========== */
%%
WHITESPACE    [ \t\r]+
COMMENT       "//".*
NUMBER        [0-9]+(\.[0-9]+)?

BOOL_TRUE     "True"
BOOL_FALSE    "False"
STRING        \"[^\"\n]*\"  // Simplificada para evitar problemas con m4
NULL_VAL      "Null"

ADD           [+]
SUB           [-]
MUL           [*]
DIV           [/]
MOD           [%]

LT            [<]
GT            [>]
LE            "<="
GE            ">="
EQ            "=="
NE            "!="
AND           "&"
OR            [|]
NOT           "!"

CONCAT        "@"
CONCAT_SPACE  "@@"

PRINT         "print"
READ          "read"
PARSE         "parse"

SIN           "sin"
COS           "cos"
MAX           "max"
MIN           "min"

%%

/* ========== Reglas de Análisis Lexicográfico ========== */
{WHITESPACE}     { /* Ignorar espacios en blanco */ }
{COMMENT}        { /* Ignorar comentarios */ }

{NUMBER}         { yylval.num = std::stod(yytext); return NUMBER; }
{BOOL_TRUE}      { yylval.boolean = true; return BOOL; }
{BOOL_FALSE}     { yylval.boolean = false; return BOOL; }
{STRING}         { 
                    yylval.str = new std::string(yytext); 
                    return STRING; 
                 }
{NULL_VAL}       { return TOKEN_NULL; }

{ADD}            { return ADD; }
{SUB}            { return SUB; }
{MUL}            { return MUL; }
{DIV}            { return DIV; }
{MOD}            { return MOD; }

{LT}             { return LT; }
{GT}             { return GT; }
{LE}             { return LE; }
{GE}             { return GE; }
{EQ}             { return EQ; }
{NE}             { return NE; }

{AND}            { return AND; }
{OR}             { return OR; }
{NOT}            { return NOT; }

{CONCAT}         { return CONCAT; }
{CONCAT_SPACE}   { return CONCAT_SPACE; }

{PRINT}          { return PRINT; }
{READ}           { return READ; }
{PARSE}          { return PARSE; }
{SIN}            { return SIN; }
{COS}            { return COS; }
{MAX}            { return MAX; }
{MIN}            { return MIN; }

.                { 
                    if (yytext && yytext[0] != '\0') {
                        std::cerr << "Carácter inesperado: " << yytext << std::endl; 
                    } else {
                        std::cerr << "Carácter inesperado: carácter nulo o vacío" << std::endl;
                    }
                    exit(1); 
                 }

%%


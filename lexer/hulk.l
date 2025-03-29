%option c++ noyywrap

%{
#include "../parser/parser.hpp"  // Para la integración con Bison
#include <iostream>
#include <cstdlib>
#include <string>

// Definimos una clase Lexer para encapsular yylex()
class Lexer {
public:
    std::string text;
};

// Declaramos el token y la instancia de la clase Lexer
#define YY_DECL HULK::Parser::token_type yylex(HULK::Parser::semantic_type* yylval)

%}

/* ========== Definición de Expresiones Regulares ========== */
%%
WHITESPACE    [ \t\r]+
COMMENT       "//".*

NUMBER        [0-9]+(\.[0-9]+)?      
BOOL_TRUE     "True"                 
BOOL_FALSE    "False"                
STRING        "\"([^\"\\]|\\.)*\""   
NULL_VAL      "Null"                 

ADD           "\+"
SUB           "-"
MUL           "\*"
DIV           "/"
MOD           "%"

LT            "<"
GT            ">"
LE            "<="
GE            ">="
EQ            "=="
NE            "!="

AND           "&"
OR            "|"
NOT           "!"

CONCAT        "@"
CONCAT_SPACE  "@@"

PRINT         "print"
READ          "read"
PARSE         "parse"
SIN           "sin"
COS           "cos"
MIN           "min"
MAX           "max"

%%

/* ========== Reglas de Análisis Lexicográfico ========== */
{WHITESPACE}     { /* Ignorar espacios en blanco */ }
{COMMENT}        { /* Ignorar comentarios */ }

{NUMBER}         { yylval->num = std::stod(yytext); return HULK::Parser::token_type::NUMBER; }
{BOOL_TRUE}      { yylval->boolean = true; return HULK::Parser::token_type::BOOL; }
{BOOL_FALSE}     { yylval->boolean = false; return HULK::Parser::token_type::BOOL; }
{STRING}         { yylval->str = new std::string(yytext); return HULK::Parser::token_type::STRING; }
{NULL_VAL}       { return HULK::Parser::token_type::TOKEN_NULL; }

{ADD}            { return HULK::Parser::token_type::ADD; }
{SUB}            { return HULK::Parser::token_type::SUB; }
{MUL}            { return HULK::Parser::token_type::MUL; }
{DIV}            { return HULK::Parser::token_type::DIV; }
{MOD}            { return HULK::Parser::token_type::MOD; }

{LT}             { return HULK::Parser::token_type::LT; }
{GT}             { return HULK::Parser::token_type::GT; }
{LE}             { return HULK::Parser::token_type::LE; }
{GE}             { return HULK::Parser::token_type::GE; }
{EQ}             { return HULK::Parser::token_type::EQ; }
{NE}             { return HULK::Parser::token_type::NE; }

{AND}            { return HULK::Parser::token_type::AND; }
{OR}             { return HULK::Parser::token_type::OR; }
{NOT}            { return HULK::Parser::token_type::NOT; }

{CONCAT}         { return HULK::Parser::token_type::CONCAT; }
{CONCAT_SPACE}   { return HULK::Parser::token_type::CONCAT_SPACE; }

{PRINT}          { return HULK::Parser::token_type::PRINT; }
{READ}           { return HULK::Parser::token_type::READ; }
{PARSE}          { return HULK::Parser::token_type::PARSE; }
{SIN}            { return HULK::Parser::token_type::SIN; }
{COS}            { return HULK::Parser::token_type::COS; }
{MAX}            { return HULK::Parser::token_type::MAX; }
{MIN}            { return HULK::Parser::token_type::MIN; }

.                { std::cerr << "Carácter inesperado: " << yytext << std::endl; exit(1); }

%%
// User-defined code can go here if needed.
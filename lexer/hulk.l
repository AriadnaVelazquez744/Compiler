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

%class Lexer
%option noyywrap

/* ========== Definición de Expresiones Regulares ========== */
WHITESPACE   [ \t\r]+
COMMENT      "//".*

NUMBER       [0-9]+(\.[0-9]+)?      
BOOL_TRUE    "True"                 
BOOL_FALSE   "False"                
STRING       "\"([^\"\\]|\\.)*\""   
NULL_VAL     "Null"                 

ADD          "\+"
SUB          "-"
MUL          "\*"
DIV          "/"
MOD          "%"

LT           "<"
GT           ">"
LE           "<="
GE           ">="
EQ           "=="
NE           "!="

AND          "&"
OR           "|"
NOT          "!"

CONCAT       "@"
CONCAT_SPACE "@@"

PRINT        "print"
READ         "read"
PARSE        "parse"
SIN          "sin"
COS          "cos"
MIN          "min"
MAX          "max"

/* ========== Reglas de Análisis Lexicográfico ========== */
%%
{WHITESPACE}    { /* Ignorar espacios en blanco */ }
{COMMENT}       { /* Ignorar comentarios */ }

{NUMBER}        { yylval->num = std::stod(yytext); return HULK::Parser::token::NUMBER; }
{BOOL_TRUE}     { yylval->boolean = true; return HULK::Parser::token::BOOL; }
{BOOL_FALSE}    { yylval->boolean = false; return HULK::Parser::token::BOOL; }
{STRING}        { yylval->str = new std::string(yytext); return HULK::Parser::token::STRING; }
{NULL_VAL}      { return HULK::Parser::token::TOKEN_NULL; }

{ADD}           { return HULK::Parser::token::ADD; }
{SUB}           { return HULK::Parser::token::SUB; }
{MUL}           { return HULK::Parser::token::MUL; }
{DIV}           { return HULK::Parser::token::DIV; }
{MOD}           { return HULK::Parser::token::MOD; }

{LT}            { return HULK::Parser::token::LT; }
{GT}            { return HULK::Parser::token::GT; }
{LE}            { return HULK::Parser::token::LE; }
{GE}            { return HULK::Parser::token::GE; }
{EQ}            { return HULK::Parser::token::EQ; }
{NE}            { return HULK::Parser::token::NE; }

{AND}           { return HULK::Parser::token::AND; }
{OR}            { return HULK::Parser::token::OR; }
{NOT}           { return HULK::Parser::token::NOT; }

{CONCAT}        { return HULK::Parser::token::CONCAT; }
{CONCAT_SPACE}  { return HULK::Parser::token::CONCAT_SPACE; }

{PRINT}         { return HULK::Parser::token::PRINT; }
{READ}          { return HULK::Parser::token::READ; }
{PARSE}         { return HULK::Parser::token::PARSE; }
{SIN}         { return HULK::Parser::token::SIN; }
{COS}         { return HULK::Parser::token::COS; }
{MAX}         { return HULK::Parser::token::MAX; }
{MIN}         { return HULK::Parser::token::MIN; }

.               { std::cerr << "Carácter inesperado: " << yytext << std::endl; exit(1); }
%%

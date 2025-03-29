%{
#include <iostream>  
#include <cmath>  
#include <string>  
#include <memory>  
#include <cstdlib>  
#include "../lexer/lexer.hpp"  // Integración con el lexer

// Definimos el espacio de nombres HULK
namespace HULK {
    class Parser {
    public:
        static int parse();  // Función principal de análisis
    };
}
%}

/* Configuración de Bison */
%define parse.error verbose

/* Definición de Tipos */
%union {
    double num;               // Para valores numéricos
    bool boolean;             // Para valores booleanos
    std::string* str;         // Para cadenas
    std::string* stmt;        // Tipo genérico para statement
}

/* Definición de Tokens */
%token <num> NUMBER
%token <boolean> BOOL
%token <str> STRING
%token TOKEN_NULL

/* Operadores */
%token ADD SUB MUL DIV MOD
%token LT GT LE GE EQ NE
%token AND OR NOT
%token CONCAT CONCAT_SPACE

/* Funciones y palabras clave */
%token PRINT READ PARSE SIN COS MIN MAX

/* Definición de Tipos para las Reglas Gramaticales */
%type <num> expr              // Asegura que expr sea de tipo double
%type <boolean> bool_expr
%type <str> str_expr
%type <stmt> statement        // Asociar statement a un tipo genérico

/* Precedencia de Operadores */
%left ADD SUB
%left MUL DIV MOD
%right SIN COS MIN MAX
%left LT GT LE GE EQ NE
%left AND OR
%right NOT
%nonassoc CONCAT CONCAT_SPACE

/* Punto de Entrada */
%start program

%%

/* ========== REGLAS DEL LENGUAJE ========== */

/* Programa: una secuencia de sentencias */
program:
      /* Vacío */
    | program statement
    ;

/* Sentencias */
statement:
      expr                   { std::cout << "Resultado: " << $1 << std::endl; }
    | PRINT expr             { std::cout << "Salida: " << $2 << std::endl; }
    | READ                   { std::string input; std::cin >> input; $$ = new std::string(input); }
    | PARSE STRING           { std::cout << "Parsing: " << *$2 << std::endl; $$ = $2; }
    | bool_expr              { /* Evaluación de booleano sin imprimir */ }
    | str_expr               { $$ = $1; }
    ;

/* Expresiones Numéricas */
expr:
      NUMBER                 { $$ = $1; }
    | expr ADD expr          { $$ = $1 + $3; }
    | expr SUB expr          { $$ = $1 - $3; }
    | expr MUL expr          { $$ = $1 * $3; }
    | expr DIV expr          { $$ = ($3 != 0) ? $1 / $3 : throw std::runtime_error("División por cero"); }
    | expr MOD expr          { $$ = std::fmod($1, $3); }
    | SIN expr               { $$ = std::sin($2); }
    | COS expr               { $$ = std::cos($2); }
    | MIN expr expr          { $$ = std::min($2, $3); }
    | MAX expr expr          { $$ = std::max($2, $3); }
    ;

/* Expresiones Booleanas */
bool_expr:
      BOOL                   { $$ = $1; }
    | expr LT expr           { $$ = $1 < $3; }
    | expr GT expr           { $$ = $1 > $3; }
    | expr LE expr           { $$ = $1 <= $3; }
    | expr GE expr           { $$ = $1 >= $3; }
    | expr EQ expr           { $$ = $1 == $3; }
    | expr NE expr           { $$ = $1 != $3; }
    | bool_expr AND bool_expr { $$ = $1 && $3; }
    | bool_expr OR bool_expr  { $$ = $1 || $3; }
    | NOT bool_expr          { $$ = !$2; }
    ;

/* Expresiones de Cadenas */
str_expr:
      STRING                 { $$ = new std::string(*$1); delete $1; }
    | str_expr CONCAT str_expr { $$ = new std::string(*$1 + *$3); delete $1; delete $3; }
    | str_expr CONCAT_SPACE str_expr { $$ = new std::string(*$1 + " " + *$3); delete $1; delete $3; }
    | TOKEN_NULL             { $$ = new std::string("Null"); }
    | '(' str_expr ')'       { $$ = $2; }
    ;

%%

/* ========== FUNCIONES AUXILIARES ========== */

/* Manejo de errores de parsing */
void yyerror(const char* s) {
    std::cerr << "Error de parsing: " << s << std::endl;
}

/* Función para leer una entrada desde la consola */
std::string read_input() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

/* Función para procesar una entrada */
std::string* parse_input(const std::string& input) {
    return new std::string("Parsed: " + input);
}

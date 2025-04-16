%{
#include <stdio.h>

extern int yylex();
void yyerror(const char *msg);

// Definir estructura para ubicación
typedef struct YYLTYPE {
    int first_line;
    int first_column;
    int last_line;
    int last_column;
} YYLTYPE;
#define YYLTYPE_IS_DECLARED 1

%}

%code requires {
    #include <string>
    #include <iostream>
    #include <cmath>
}

// Habilitar seguimiento de ubicaciones
%locations

// Definir la unión de tipos semánticos
%union {
    double num;  // Tipo para números (enteros y decimales)
    std::string* str; 
    bool boolean;
    std::string* stmt;
}

// --------------------------------------/* Definición de Tokens */------------------------------------------- //

// Literales 
%token <num> NUMBER
%token <str> STRING
%token <boolean> BOOL
%token NULL_VAL

%token ';'
%token '(' ')'

// operadores aritméticos
%token ADD
%token SUB
%token MUL
%token DIV
%token MOD

// operadores booleanos entre exprresiones numéricas
%token LT
%token GT 
%token LE 
%token GE 
%token EQ
%token NE

// operadores booleanos entre exprresiones booleanas
%token AND
%token OR 
%token NOT

// operadores para literales de texto
%token CONCAT
%token CONCAT_SPACE

// funciones matemáticas básicas
%token SIN
%token COS
%token MAX
%token MIN
%token POW
%token SQRT
%token EXP
%token LOG
%token RANDOM


// funciones predeterminadas
%token PRINT
%token READ

// -----------------------------/* Definición de Tipos para las Reglas Gramaticales */------------------------ //
%type <stmt> statement
%type <num> expr
%type <str> str_expr
%type <boolean> bool_expr
%type <stmt> null_expr
%type <stmt> value

// ---------------------------------------/* Precedencia de Operadores */------------------------------------- //
%left ADD SUB
%left MUL DIV MOD
%right SIN COS MIN MAX SQRT LOG EXP RANDOM POW
%left LT GT LE GE EQ NE
%left AND OR 
%right NOT
%nonassoc CONCAT CONCAT_SPACE

%%

program:
    /* vacío */
    | program statement
    | program error ';' { yyerrok; }
;

statement:
    expr ';'                { std::cout << "Resultado: " << $1 << std::endl; }
    | str_expr ';'          { std::cout << "Texto: " << $1->c_str() << std::endl; delete $1; }
    | bool_expr ';'         { std::cout << "Booleano: " << ($1 ? "true" : "false") << std::endl; }
    | null_expr ';'         { std::cout << "Null valor reconocido\n"; delete $1; }
    | PRINT value ';'       { std::cout << "Salida: " << *$2 << std::endl; }
    | READ ';'              { 
                                std::string input; 
                                //std::cin >> input;
                                std::getline(std::cin, input); 
                                //$$ = new std::string(input); 
                                std::cout << "Entrada: " << input << std::endl;
                            }

;

    value:
        expr                { $$ = new std::string(std::to_string($1)); }
        | str_expr          { $$ = new std::string(*$1); delete $1; }
        | bool_expr         { $$ = new std::string($1 ? "true" : "false"); }
        | null_expr         { $$ = new std::string("null"); }
        | '(' value ')'     { $$ = $2; }
    ;

    expr:
        NUMBER              { $$ = $1; printf("Número reconocido: %g\n", $$); }
        | expr ADD expr     { $$ = $1 + $3; printf("%g + %g\n", $1, $3); }
        | expr SUB expr     { $$ = $1 - $3; printf("%g - %g\n", $1, $3); }
        | expr MUL expr     { $$ = $1 * $3; printf("%g * %g\n", $1, $3); }
        | expr DIV expr     { 
                                $$ = ($3 != 0) ? $1 / $3 : throw std::runtime_error("División por cero"); 
                                printf("%g / %g\n", $1, $3);
                            }
        | expr MOD expr     { $$ = std::fmod($1, $3); printf("mod ( %g, %g )\n", $1, $3); }
        | SUB expr          { $$ = - $2; printf("Número negativo: %g\n", $$); }
        | SIN expr          { $$ = std::sin($2); }
        | COS expr          { $$ = std::cos($2); }
        | MIN expr expr     { $$ = std::min($2, $3); }
        | MAX expr expr     { $$ = std::max($2, $3); }
        | SQRT expr         { $$ = std::sqrt($2); }
        | LOG expr          { $$ = std::log($2); }
        | EXP expr          { $$ = std::exp($2); }
        | RANDOM            { $$ = rand() / (RAND_MAX + 1.0); }
        | POW expr expr     { $$ = std::pow($2, $3); }
    ;

    str_expr:
        STRING                              {   
                                                $$ = new std::string(*$1);
                                                printf("Texto reconocido: %s\n", $$->c_str()); 
                                                delete $1;  // Clean up allocated string
                                            }
        | str_expr CONCAT str_expr          { $$ = new std::string(*$1 + *$3); delete $1; delete $3; }
        | str_expr CONCAT_SPACE str_expr    { $$ = new std::string(*$1 + " " + *$3); delete $1; delete $3; }
    ;

    bool_expr:
        BOOL                        { $$ = $1; printf("Booleano: %s\n", $$ ? "true" : "false"); }
        | expr LT expr              { $$ = $1 < $3; }
        | expr GT expr              { $$ = $1 > $3; }
        | expr LE expr              { $$ = $1 <= $3; }
        | expr GE expr              { $$ = $1 >= $3; }
        | value EQ value            { $$ = (*$1 == *$3); delete $1; delete $3; }
        | value NE value            { $$ = (*$1 != *$3); delete $1; delete $3; }
        | bool_expr AND bool_expr   { $$ = $1 && $3; }
        | bool_expr OR bool_expr    { $$ = $1 || $3; }
        | NOT bool_expr             { $$ = !$2; }
    ;

    null_expr:
        NULL_VAL     { $$ = new std::string("null"); }
    ;

%%

void yyerror(const char *msg) {
    fprintf(stderr, "Error en línea %d, columna %d: %s\n",
            yylloc.first_line, yylloc.first_column, msg);
}



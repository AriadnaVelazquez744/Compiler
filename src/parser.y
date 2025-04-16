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

// operadores booleanos entre expresiones numéricas
%token LT
%token GT 
%token LE 
%token GE 
%token EQ
%token NE

// operadores booleanos entre expresiones booleanas
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

// -----------------------------/* Definición de Tipos para las Reglas Gramaticales */------------------------ //
%type <stmt> statement
%type <num> exp
%type <str> str_exp
%type <boolean> bool_exp
%type <stmt> null_exp

// ---------------------------------------/* Precedencia de Operadores */------------------------------------- //
%left ADD SUB
%left MUL DIV MOD
%right SIN COS MIN MAX
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
    exp ';'         { std::cout << "Resultado: " << $1 << std::endl; }
    | str_exp ';'   { std::cout << "Texto: " << $1->c_str() << std::endl; delete $1; }
    | bool_exp ';'  { std::cout << "Booleano: " << ($1 ? "true" : "false") << std::endl; }
    | null_exp ';'
;

    exp:
        NUMBER          { $$ = $1; printf("Número reconocido: %g\n", $$); }
        | exp ADD exp   { $$ = $1 + $3; printf("%g + %g\n", $1, $3); }
        | exp SUB exp   { $$ = $1 - $3; printf("%g - %g\n", $1, $3); }
        | exp MUL exp   { $$ = $1 * $3; printf("%g * %g\n", $1, $3); }
        | exp DIV exp   { 
                            $$ = ($3 != 0) ? $1 / $3 : throw std::runtime_error("División por cero"); 
                            printf("%g / %g\n", $1, $3);
                        }
        | exp MOD exp   { $$ = std::fmod($1, $3); printf("mod ( %g, %g )\n", $1, $3); }
        | SUB exp       { $$ = - $2; printf("Número negativo: %g\n", $$); }
        | '(' exp ')'   { $$ = $2; printf("( %g )\n", $2); }
        | SIN exp       { $$ = std::sin($2); }
        | COS exp       { $$ = std::cos($2); }
        | MIN exp exp   { $$ = std::min($2, $3); }
        | MAX exp exp   { $$ = std::max($2, $3); }

    ;

    str_exp:
        STRING  {   
                    $$ = new std::string(*$1);
                    printf("Texto reconocido: %s\n", $$->c_str()); 
                    delete $1;  // Clean up allocated string
                }
        | str_exp CONCAT str_exp { $$ = new std::string(*$1 + *$3); delete $1; delete $3; }
        | str_exp CONCAT_SPACE str_exp { $$ = new std::string(*$1 + " " + *$3); delete $1; delete $3; }
    
    ;

    bool_exp:
        BOOL { $$ = $1; printf("Booleano: %s\n", $$ ? "true" : "false"); }
        | exp LT exp           { $$ = $1 < $3; }
        | exp GT exp           { $$ = $1 > $3; }
        | exp LE exp           { $$ = $1 <= $3; }
        | exp GE exp           { $$ = $1 >= $3; }
        | exp EQ exp           { $$ = $1 == $3; }
        | exp NE exp           { $$ = $1 != $3; }
        | bool_exp AND bool_exp { $$ = $1 && $3; }
        | bool_exp OR bool_exp  { $$ = $1 || $3; }
        | NOT bool_exp          { $$ = !$2; }
        | '(' bool_exp ')'     { $$ = $2; }
    ;

    null_exp:
        NULL_VAL ';'     { std::cout << "Null valor reconocido\n"; }
    ;


%%

void yyerror(const char *msg) {
    fprintf(stderr, "Error en línea %d, columna %d: %s\n",
            yylloc.first_line, yylloc.first_column, msg);
}



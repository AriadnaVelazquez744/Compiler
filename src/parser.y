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

#define PI_VAL 3.14159265358979323846
#define TRACE(EXPR) std::cout << "elem_expr: " << *EXPR << std::endl;

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
%token <str> ID

%token ',' ';'
%token '(' ')'
%token '{' '}'

// operadores aritméticos
%token ADD
%token SUB
%token MUL
%token DIV
%token MOD
%token POW

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
%token SQRT
%token EXP
%token LOG
%token RANDOM

// funciones predeterminadas
%token PRINT
%token READ
%token PARSE

// constantes 
%token PI 
%token E

// -----------------------------/* Definición de Tipos para las Reglas Gramaticales */------------------------ //
%type <stmt> statement
%type <stmt> expression
%type <stmt> elem_expr
%type <stmt> block_expr
/* %type <stmt> block_body */
%type <stmt> block_body

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
    expression ';'                  { }
    | PRINT '(' expression ')' ';'  { std::cout << "Salida: " << *$3 << std::endl; }
    | READ ';'                      { 
                                        std::string input; 
                                        //std::cin >> input;
                                        std::getline(std::cin, input); 
                                        //$$ = new std::string(input); 
                                        std::cout << "Entrada: " << input << std::endl;
                                    }
    | PARSE '(' expression ')' ';'  {
                                        const std::string& raw = *$3;
                                        // Try parse as number
                                        try {
                                            double num = std::stod(raw);
                                            std::cout << "Parseado como número: " << num << std::endl;
                                        } catch (...) {
                                            // Try parse as boolean
                                            if (raw == "true" || raw == "True")
                                                std::cout << "Parseado como booleano: true" << std::endl;
                                            else if (raw == "false" || raw == "False")
                                                std::cout << "Parseado como booleano: false" << std::endl;
                                            else
                                                std::cout << "Parse fallido: " << raw << std::endl;
                                        }
                                        delete $3;
                                    }
    | block_expr
;

    expression:
          NUMBER        { $$ = new std::string(std::to_string($1)); }
        | STRING        { $$ = new std::string(*$1); delete $1; }
        | BOOL          { $$ = new std::string($1 ? "true" : "false"); }
        | NULL_VAL      { $$ = new std::string("null"); }
        | ID            { $$ = $1; }
        | elem_expr     { $$ = $1; }
        | block_expr    { $$ = $1; }
    ;

        elem_expr:
              expression ADD expression {
                $$ = new std::string("(" + *$1 + " + " + *$3 + ")");
                delete $1; delete $3;
                TRACE($$);
            }
            | expression SUB expression {
                $$ = new std::string("(" + *$1 + " - " + *$3 + ")");
                delete $1; delete $3;
                TRACE($$);
            }
            | expression MUL expression {
                $$ = new std::string("(" + *$1 + " * " + *$3 + ")");
                delete $1; delete $3;
                TRACE($$);
            }
            | expression DIV expression {
                $$ = new std::string("(" + *$1 + " / " + *$3 + ")");
                delete $1; delete $3;
                TRACE($$);
            }
            | expression MOD expression {
                $$ = new std::string("mod(" + *$1 + ", " + *$3 + ")");
                delete $1; delete $3;
                TRACE($$);
            }
            | expression POW expression {
                $$ = new std::string("pow(" + *$1 + ", " + *$3 + ")");
                delete $1; delete $3;
                TRACE($$);
            }

            | SUB NUMBER {
                $$ = new std::string("-" + std::to_string($2));
                TRACE($$);
            }

            | '(' expression ')' {
                $$ = $2;    TRACE($$);
            }

            | SIN '(' expression ')' {
                $$ = new std::string("sin(" + *$3 + ")");
                delete $3;
                TRACE($$);
            }
            | COS '(' expression ')' {
                $$ = new std::string("cos(" + *$3 + ")");
                delete $3;
                TRACE($$);
            }
            | MIN '(' expression ',' expression ')' {
                $$ = new std::string("min(" + *$3 + ", " + *$5 + ")");
                delete $3; delete $5;
                TRACE($$);
            }
            | MAX '(' expression ',' expression ')' {
                $$ = new std::string("max(" + *$3 + ", " + *$5 + ")");
                delete $3; delete $5;
                TRACE($$);
            }
            | SQRT '(' expression ')' {
                $$ = new std::string("sqrt(" + *$3 + ")");
                delete $3;
                TRACE($$);
            }
            | LOG '(' expression ',' expression ')' {
                $$ = new std::string("log(" + *$3 + ", " + *$5 + ")");
                delete $3; delete $5;
                TRACE($$);
            }
            | EXP '(' expression ')' {
                $$ = new std::string("exp(" + *$3 + ")");
                delete $3;
                TRACE($$);
            }

            | RANDOM '(' ')' {
                $$ = new std::string("random()");
                TRACE($$);
            }
            | E {
                $$ = new std::string("e");
                TRACE($$);
            }
            | PI {
                $$ = new std::string("pi");
                TRACE($$);
            }

            | expression CONCAT expression {
                $$ = new std::string(*$1 + *$3); // string concatenation
                delete $1; delete $3;
                TRACE($$);
            }
            | expression CONCAT_SPACE expression {
                $$ = new std::string(*$1 + " " + *$3);
                delete $1; delete $3;
                TRACE($$);
            }

            | expression LT expression {
                $$ = new std::string("(" + *$1 + " < " + *$3 + ")");
                delete $1; delete $3;
                TRACE($$);
            }
            | expression GT expression {
                $$ = new std::string("(" + *$1 + " > " + *$3 + ")");
                delete $1; delete $3;
                TRACE($$);
            }
            | expression LE expression {
                $$ = new std::string("(" + *$1 + " <= " + *$3 + ")");
                delete $1; delete $3;
                TRACE($$);
            }
            | expression GE expression {
                $$ = new std::string("(" + *$1 + " >= " + *$3 + ")");
                delete $1; delete $3;
                TRACE($$);
            }

            | expression EQ expression {
                $$ = new std::string("(" + *$1 + " == " + *$3 + ")");
                delete $1; delete $3;
                TRACE($$);
            }
            | expression NE expression {
                $$ = new std::string("(" + *$1 + " != " + *$3 + ")");
                delete $1; delete $3;
                TRACE($$);
            }

            | expression AND expression {
                $$ = new std::string("(" + *$1 + " && " + *$3 + ")");
                delete $1; delete $3;
                TRACE($$);
            }
            | expression OR expression {
                $$ = new std::string("(" + *$1 + " || " + *$3 + ")");
                delete $1; delete $3;
                TRACE($$);
            }
            | NOT expression {
                $$ = new std::string("(!" + *$2 + ")");
                delete $2;
                TRACE($$);
            }
        ;

        block_expr:
            '{' block_body '}'  { $$ = $2 ? $2 : new std::string("null"); /* empty block returns null */ }
        ;

        block_body:
            /* empty */                         { $$ = nullptr; }
            | statement                         { $$ = $1; }
            | block_body statement              { $$ = $2; delete $1; }
        ;

%%

void yyerror(const char *msg) {
    fprintf(stderr, "Error en línea %d, columna %d: %s\n",
            yylloc.first_line, yylloc.first_column, msg);
}



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
}

// Definir la unión de tipos semánticos
%union {
    double num;  // Tipo para números (enteros y decimales)
    std::string* str; 
    bool boolean;
    std::string* stmt;
}

// Asociar el token NUMBER al campo 'number' de la unión
%token <num> NUMBER
%token <str> STRING
%token <boolean> BOOL

%type <stmt> statement
%type <num> exp
%type <str> str_exp
%type <boolean> bool_exp

// Habilitar seguimiento de ubicaciones
%locations

%%

program:
    /* vacío */
    | program statement
;

statement:
    exp             { std::cout << "Resultado: " << $1 << std::endl; };
    | str_exp       { $$ = $1; }
    | bool_exp      { /* Evaluación de booleano sin imprimir */ }
;

    exp:
        NUMBER  {   $$ = $1; printf("Número reconocido: %g\n", $$); }
    ;

    str_exp:
        STRING  {   
                    $$ = new std::string(*$1);
                    printf("Texto reconocido: %s\n", $$->c_str()); 
                    delete $1;  // Clean up allocated string
                }
    ;

    bool_exp:
        BOOL { $$ = $1; printf("Booleano: %s\n", $$ ? "true" : "false"); }
    ;

%%

void yyerror(const char *msg) {
    fprintf(stderr, "Error en línea %d, columna %d: %s\n",
            yylloc.first_line, yylloc.first_column, msg);
}



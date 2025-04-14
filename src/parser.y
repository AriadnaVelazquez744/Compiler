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

// Definir la unión de tipos semánticos
%union {
    double num;  // Tipo para números (enteros y decimales)
}

// Asociar el token NUMBER al campo 'number' de la unión
%token <num> NUMBER

// Habilitar seguimiento de ubicaciones
%locations

%%

program:
    /* vacío */
    | program expression
;

expression:
    NUMBER { printf("Número reconocido: %g\n", $1); }
;

%%

void yyerror(const char *msg) {
    fprintf(stderr, "Error en línea %d, columna %d: %s\n",
            yylloc.first_line, yylloc.first_column, msg);
}



%{

#include <stdio.h>
#include "../include/AST.hpp"

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

ASTNode* root;

#define PI_VAL 3.14159265358979323846
#define TRACE(EXPR) std::cout << "elem_expr: " << *EXPR << std::endl;

std::vector<ASTNode*> vectorize(ASTNode* arg1, ASTNode* arg2, int n) {
    
    std::vector<ASTNode*> args = std::vector<ASTNode*>();
    
    if ((n == 1) || (n == 2)) { args.push_back(arg1); }
    if (n == 2) { args.push_back(arg2); }

    return args;
}

%}

%code requires {
    #include <string>
    #include <iostream>
    #include <cmath>
    #include <vector>
    #include "../include/AST.hpp"
}

// Habilitar seguimiento de ubicaciones
%locations

// Definir la unión de tipos semánticos
%union {
    double num;  // Tipo para números (enteros y decimales)
    std::string* str; 
    bool boolean;
    ASTNode* node;
    std::vector<ASTNode*>* list;
    std::vector<IfBranch>* if_branch;
    std::vector<Parameter>* param;
    std::vector<LetDeclaration>* let_decl;
}

// --------------------------------------/* Definición de Tokens */------------------------------------------- //

// Literales 
%token <num> NUMBER
%token <str> STRING
%token <boolean> BOOL
%token NULL_VAL
%token <str> ID

%token ',' ';' '.'
%token '(' ')'
%token '{' '}'
%token LAMBDA 
%token '=' REASSIGN

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

// palabras clave
%token FUNC
%token LET 
%token IN
%token IF 
%token ELIF 
%token ELSE
%token FOR 
%token WHILE 
%token RANGE
%token TYPE 
%token NEW 
%token SELF
%token INHERITS

// -----------------------------/* Definición de Tipos para las Reglas Gramaticales */------------------------ //
%type <node> statement
%type <node> expression
%type <node> elem_expr
%type <node> block_expr
%type <node> func_call_expr
%type <node> assign_expr
%type <node> let_expr
%type <node> if_expr
%type <node> while_expr 
%type <node> for_expr
%type <node> body
%type <list> block_body
%type <list> args
%type <param> params
%type <if_branch> if_head
%type <let_decl> decl

// ---------------------------------------/* Precedencia de Operadores */------------------------------------- //
%left ADD SUB
%left MUL DIV MOD
%right  POW SIN COS MIN MAX SQRT LOG EXP RANDOM
%left LT GT LE GE EQ NE
%left AND OR 
%right NOT
%nonassoc CONCAT CONCAT_SPACE

%%

program:
    /* vacío */
    | program statement             { root = $2; }
    | program error ';'             { yyerrok; }
;

statement:
    expression ';'                  { $$ = $1; delete $1; }
    | PRINT '(' expression ')' ';'  { 
                                        std::vector<ASTNode*> args = vectorize($3, nullptr, 1);
                                        $$ = new BuiltInFunctionNode("print", args, yylloc.first_line);
                                    }
    /* | READ ';'                      { 
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
                                    } */
    | block_expr                    { $$ = $1; }
    | FUNC ID '(' params ')' LAMBDA body
                                    {
                                        $$ = new FunctionDeclarationNode(*$2, $4, $7, true, yylloc.first_line);
                                        std::cout << "Definición función inline: " << *$2 << std::endl;
                                        delete $2; delete $4;
                                    }
    | FUNC ID '(' params ')' block_expr
                                    {
                                        $$ = new FunctionDeclarationNode(*$2, $4, $6, false, yylloc.first_line);
                                        std::cout << "Definición función bloque: " << *$2 << std::endl;
                                        delete $2; delete $4;
                                    }
    | let_expr                      { $$ = $1; std::cout << "let_expr " << std::endl; }
    | while_expr                    { $$ = $1; std::cout << "while_expr " << std::endl; }
    | for_expr                      { $$ = $1; std::cout << "for_expr " << std::endl; }
;

    expression:
          NUMBER                { $$ = new LiteralNode(std::to_string($1), "Number", yylloc.first_line); }
        | STRING                { $$ = new LiteralNode(*$1, "String", yylloc.first_line); delete $1; }
        | BOOL                  { $$ = new LiteralNode($1 ? "true" : "false", "Boolean", yylloc.first_line); }
        | NULL_VAL              { $$ = new LiteralNode("null", "Null", yylloc.first_line); }
        | ID                    { $$ = new IdentifierNode(*$1, yylloc.first_line); }
        | elem_expr             { $$ = $1; }
        | block_expr            { $$ = $1; }
        | func_call_expr        { $$ = $1; }
        | assign_expr           { $$ = $1; }
        | let_expr              { $$ = $1; std::cout << "let_expr " << std::endl; }
        | if_expr               { $$ = $1; std::cout << "if_expr " << std::endl; }
        | while_expr            { $$ = $1; std::cout << "while_expr " << std::endl; }
        | for_expr              { $$ = $1; std::cout << "for_expr " << std::endl; }
    ;

        elem_expr:
              expression ADD expression {
                $$ = new BinaryOpNode("+", $1, $3, yylloc.first_line);
                delete $1; delete $3;
            }
            | expression SUB expression {
                $$ = new BinaryOpNode("-", $1, $3, yylloc.first_line);
                delete $1; delete $3;
            }
            | expression MUL expression {
                $$ = new BinaryOpNode("*", $1, $3, yylloc.first_line);
                delete $1; delete $3;
            }
            | expression DIV expression {
                $$ = new BinaryOpNode("/", $1, $3, yylloc.first_line);
                delete $1; delete $3;
            }
            | expression MOD expression {
                
                $$ = new BinaryOpNode("%", $1, $3, yylloc.first_line);
                delete $1; delete $3;
            }
            | expression POW expression {
                $$ = new BinaryOpNode("^", $1, $3, yylloc.first_line);
                delete $1; delete $3;
            }

            | SUB expression {
                $$ = new UnaryOpNode("-", $2, yylloc.first_line);
                delete $2;
            }

            | '(' expression ')' {
                $$ = $2;
            }

            /* | SIN '(' expression ')' {
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
            } */
            | E     { $$ = new IdentifierNode("e", yylloc.first_line); }
            | PI    { $$ = new IdentifierNode("pi", yylloc.first_line); }

            | expression CONCAT expression {
                $$ = new BinaryOpNode("@", $1, $3, yylloc.first_line);
                delete $1; delete $3;
            }
            | expression CONCAT_SPACE expression {
                $$ = new BinaryOpNode("@@", $1, $3, yylloc.first_line);
                delete $1; delete $3;
            }

            | expression LT expression {
                
                $$ = new BinaryOpNode("<", $1, $3, yylloc.first_line);
                delete $1; delete $3;
            }
            | expression GT expression {
                $$ = new BinaryOpNode(">", $1, $3, yylloc.first_line);
                delete $1; delete $3;
            }
            | expression LE expression {
                $$ = new BinaryOpNode("<=", $1, $3, yylloc.first_line);
                delete $1; delete $3;
            }
            | expression GE expression {
                $$ = new BinaryOpNode(">=", $1, $3, yylloc.first_line);
                delete $1; delete $3;
            }

            | expression EQ expression {
                $$ = new BinaryOpNode("==", $1, $3, yylloc.first_line);
                delete $1; delete $3;
            }
            | expression NE expression {
                $$ = new BinaryOpNode("!=", $1, $3, yylloc.first_line);
                delete $1; delete $3;
            }

            | expression AND expression {
                $$ = new BinaryOpNode("&", $1, $3, yylloc.first_line);
                delete $1; delete $3;
            }
            | expression OR expression {
                $$ = new BinaryOpNode("|", $1, $3, yylloc.first_line);
                delete $1; delete $3;
            }
            | NOT expression {
                $$ = new UnaryOpNode("!", $2, yylloc.first_line);
                delete $2;
            }
        ;

        block_expr:
            '{' block_body '}'  {
                                    $$ = new BlockNode(*$2, yylloc.first_line); // Placeholder
                                    delete $2;
                                }
        ;

        block_body:
            /* empty */                     { $$ = new std::vector<ASTNode*>(); }
            | statement                     { $$ = new std::vector<ASTNode*>(); $$->push_back($1); }
            | block_body statement          { $1->push_back($2); $$ = $1; }
        ;

        params:
            /* empty */         { $$ = new std::vector<Parameter>(); }
            | ID                { 
                                    Parameter p;
                                    p.name = *$1;
                                    $$ = new std::vector<Parameter>(); 
                                    $$->push_back(p); 
                                    delete $1;
                                }
            | params ',' ID     { 
                                    Parameter p;
                                    p.name = *$3;
                                    $1->push_back(p); 
                                    $$ = $1; 
                                }
        ;

        func_call_expr:
            ID '(' args ')'     { $$ = new FunctionCallNode(*$1, *$3, yylloc.first_line); delete $1; delete $3;}
        ;

        args: 
            /* empty */                 { $$ = new std::vector<ASTNode*>(); }
            | expression                { $$ = new std::vector<ASTNode*>(); $$->push_back($1); }
            | args ',' expression       { $1->push_back($3); $$ = $1; }
        ;

        assign_expr:
            ID REASSIGN expression    { $$ = new AssignmentNode(*$1, $3, yylloc.first_line); delete $1; }
        ;

        let_expr:
              LET decl IN body                { $$ = new LetNode($2, $4, yylloc.first_line); delete $2; }
            | LET decl IN '(' body ')'        { $$ = new LetNode($2, $5, yylloc.first_line); delete $2; }
            | LET decl IN body ';'            { $$ = new LetNode($2, $4, yylloc.first_line); delete $2; }
            | LET decl IN '(' body ')' ';'    { $$ = new LetNode($2, $5, yylloc.first_line); delete $2; }
        ;

        decl:
              ID '=' expression             {
                                                LetDeclaration d;
                                                d.name = *$1;
                                                d.initializer = $3;
                                                $$ = new std::vector<LetDeclaration>();
                                                $$->push_back(d); delete $1;
                                            }
            | decl ',' ID '=' expression    {
                                                LetDeclaration d;
                                                d.name = *$3;
                                                d.initializer = $5;
                                                $1->push_back(d); $$ = $1;
                                                delete $3;
                                            }
        ;

        body:
              statement                     { $$ = $1; }
            | expression                    { $$ = $1; }
            | PRINT '(' expression ')'      { 
                                                std::vector<ASTNode*> args = vectorize($3, nullptr, 1);
                                                $$ = new BuiltInFunctionNode("print", args, yylloc.first_line);
                                            }
        ;

        if_expr:
            if_head                     { $$ = new IfNode($1, nullptr, yylloc.first_line); }
            | if_head ELSE body         { $$ = new IfNode($1, $3, yylloc.first_line); }
        ;

        if_head:
            IF '(' expression ')' body                      { 
                                                                IfBranch b;
                                                                b.condition = $3;
                                                                b.body = $5;
                                                                $$ = new std::vector<IfBranch>(); 
                                                                $$->push_back(b);
                                                            }
            | if_head ELIF '(' expression ')' body          { 
                                                                IfBranch b;
                                                                b.condition = $4;
                                                                b.body = $6;
                                                                $1->push_back(b); 
                                                                $$ = $1;  
                                                            }
        ;

        while_expr:
            WHILE '(' expression ')' body                   { $$ = new WhileNode($3, $5, yylloc.first_line); }
        ;

        for_expr:
            FOR '(' ID IN RANGE '(' expression ',' expression ')' ')' body      { $$ = new ForNode(*$3, $7, $9, $12, yylloc.first_line); delete $3; }
        ;

%%

void yyerror(const char *msg) {
    fprintf(stderr, "Error en línea %d, columna %d: %s\n",
            yylloc.first_line, yylloc.first_column, msg);
}



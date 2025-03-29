#include "lexer.hpp"
#include "../parser/parser.hpp"
#include "../parser/hulk.tab.hpp" // Incluye los tokens generados por Bison
#include <iostream>

namespace HULK {
    /**
     * Función que ejecuta el analizador léxico.
     * Se encarga de llamar a `yylex()` en un bucle y 
     * mostrar los tokens que reconoce.
     */
    void runLexer() {
        std::cout << "Ejecutando lexer...\n";
        Lexer lexer;  // Se instancia el lexer
        int token;

        // Se procesa la entrada hasta que yylex() retorne 0 (fin del archivo)
        while ((token = lexer.yylex())) {
            switch (token) {
                // ====== Tipos básicos ======
                case NUMBER:
                    std::cout << "NUMBER" << std::endl;
                    break;
                case BOOL:
                    std::cout << "BOOL" << std::endl;
                    break;
                case STRING:
                    std::cout << "STRING" << std::endl;
                    break;
                case TOKEN_NULL:
                    std::cout << "NULL" << std::endl;
                    break;

                // ====== Funciones globales ======
                case PRINT:
                    std::cout << "PRINT" << std::endl;
                    break;
                case READ:
                    std::cout << "READ" << std::endl;
                    break;
                case PARSE:
                    std::cout << "PARSE" << std::endl;
                    break;
                case SIN:
                    std::cout << "SIN" << std::endl;
                    break;
                case COS:
                    std::cout << "COS" << std::endl;
                    break;
                case MAX:
                    std::cout << "MAX" << std::endl;
                    break;
                case MIN:
                    std::cout << "MIN" << std::endl;
                    break;


                // ====== Operadores aritméticos ======
                case ADD:
                    std::cout << "ADD (+)" << std::endl;
                    break;
                case SUB:
                    std::cout << "SUB (-)" << std::endl;
                    break;
                case MUL:
                    std::cout << "MUL (*)" << std::endl;
                    break;
                case DIV:
                    std::cout << "DIV (/)" << std::endl;
                    break;
                case MOD:
                    std::cout << "MOD (%)" << std::endl;
                    break;

                // ====== Operadores de comparación ======
                case LT:
                    std::cout << "LT (<)" << std::endl;
                    break;
                case GT:
                    std::cout << "GT (>)" << std::endl;
                    break;
                case LE:
                    std::cout << "LE (<=)" << std::endl;
                    break;
                case GE:
                    std::cout << "GE (>=)" << std::endl;
                    break;
                case EQ:
                    std::cout << "EQ (==)" << std::endl;
                    break;
                case NE:
                    std::cout << "NE (!=)" << std::endl;
                    break;

                // ====== Operadores lógicos ======
                case AND:
                    std::cout << "AND (&)" << std::endl;
                    break;
                case OR:
                    std::cout << "OR (|)" << std::endl;
                    break;
                case NOT:
                    std::cout << "NOT (!)" << std::endl;
                    break;

                // ====== Operadores de concatenación ======
                case CONCAT:
                    std::cout << "CONCAT (@)" << std::endl;
                    break;
                case CONCAT_SPACE:
                    std::cout << "CONCAT_SPACE (@@)" << std::endl;
                    break;

                // ====== Tokens desconocidos ======
                default:
                    std::cout << "Token desconocido" << std::endl;
                    break;
            }
        }
    }
}

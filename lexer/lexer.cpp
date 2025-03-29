#include "lexer.hpp"
#include "../parser/parser.hpp"
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
                case Parser::token::NUMBER:
                    std::cout << "NUMBER" << std::endl;
                    break;
                case Parser::token::BOOL:
                    std::cout << "BOOL" << std::endl;
                    break;
                case Parser::token::STRING:
                    std::cout << "STRING" << std::endl;
                    break;
                case Parser::token::TOKEN_NULL:
                    std::cout << "NULL" << std::endl;
                    break;

                // ====== Funciones globales ======
                case Parser::token::PRINT:
                    std::cout << "PRINT" << std::endl;
                    break;
                case Parser::token::READ:
                    std::cout << "READ" << std::endl;
                    break;
                case Parser::token::PARSE:
                    std::cout << "PARSE" << std::endl;
                    break;
                case Parser::token::PARSE:
                    std::cout << "PARSE" << std::endl;
                    break;
                case Parser::token::SIN:
                    std::cout << "SIN" << std::endl;
                    break;
                case Parser::token::COS:
                    std::cout << "COS" << std::endl;
                    break;
                case Parser::token::MAX:
                    std::cout << "MAX" << std::endl;
                    break;
                case Parser::token::MIN:
                    std::cout << "MIN" << std::endl;
                    break;


                // ====== Operadores aritméticos ======
                case Parser::token::ADD:
                    std::cout << "ADD (+)" << std::endl;
                    break;
                case Parser::token::SUB:
                    std::cout << "SUB (-)" << std::endl;
                    break;
                case Parser::token::MUL:
                    std::cout << "MUL (*)" << std::endl;
                    break;
                case Parser::token::DIV:
                    std::cout << "DIV (/)" << std::endl;
                    break;
                case Parser::token::MOD:
                    std::cout << "MOD (%)" << std::endl;
                    break;

                // ====== Operadores de comparación ======
                case Parser::token::LT:
                    std::cout << "LT (<)" << std::endl;
                    break;
                case Parser::token::GT:
                    std::cout << "GT (>)" << std::endl;
                    break;
                case Parser::token::LE:
                    std::cout << "LE (<=)" << std::endl;
                    break;
                case Parser::token::GE:
                    std::cout << "GE (>=)" << std::endl;
                    break;
                case Parser::token::EQ:
                    std::cout << "EQ (==)" << std::endl;
                    break;
                case Parser::token::NE:
                    std::cout << "NE (!=)" << std::endl;
                    break;

                // ====== Operadores lógicos ======
                case Parser::token::AND:
                    std::cout << "AND (&)" << std::endl;
                    break;
                case Parser::token::OR:
                    std::cout << "OR (|)" << std::endl;
                    break;
                case Parser::token::NOT:
                    std::cout << "NOT (!)" << std::endl;
                    break;

                // ====== Operadores de concatenación ======
                case Parser::token::CONCAT:
                    std::cout << "CONCAT (@)" << std::endl;
                    break;
                case Parser::token::CONCAT_SPACE:
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

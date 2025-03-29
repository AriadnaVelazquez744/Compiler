#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <variant>

namespace HULK {
    class Lexer {
    public:
        int yylex();
    };
}

#endif

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include "../parser/hulk.tab.hpp"

namespace HULK {
    class Lexer {
    public:
        int yylex();
    };
}

#endif // LEXER_H
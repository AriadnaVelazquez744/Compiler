#pragma once
#include <string>
#include "Token.hpp"

struct LexerError {
    std::string message;
    SourceLocation location;
    std::string offendingLexeme;

    LexerError(std::string msg, SourceLocation loc, std::string lexeme)
        : message(std::move(msg)), location(loc), offendingLexeme(std::move(lexeme)) {}
};

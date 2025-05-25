#pragma once
#include <string>
#include "../lexer/Token.hpp"

struct ParserError {
    std::string message;
    SourceLocation location;
    std::string expected;

    ParserError(const std::string& msg, const SourceLocation& loc, const std::string& expectedToken = "")
        : message(msg), location(loc), expected(expectedToken) {}
};

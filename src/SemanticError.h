#pragma once
#include <string>

struct SemanticError {
    std::string message;
    int line;
    SemanticError(const std::string& msg, int ln) : message(msg), line(ln) {}
};
#pragma once
#include <string>
#include <memory>

enum class TokenType {
    // Literals
    NUMBER, STRING, BOOL, NULL_VAL, ID,
    
    // Arithmetic
    ADD, SUB, MUL, DIV, MOD, POW,
    
    // Relational
    LT, GT, LE, GE, EQ, NE,
    
    // Logical
    AND, OR, NOT,
    
    // String ops
    CONCAT, CONCAT_SPACE,

    // Built-in functions
    SIN, COS, MAX, MIN, SQRT, EXP, LOG, RANDOM,

    // Keywords
    PRINT, READ, PARSE,
    FUNC, LET, IN,
    IF, ELSE, ELIF,
    FOR, WHILE, RANGE,
    TYPE, NEW, SELF, INHERITS,

    // Constants
    PI, E,

    // Punctuation
    COMMA, SEMICOLON, DOT,
    LPAREN, RPAREN,
    LBRACE, RBRACE,
    ASSIGN, REASSIGN,
    
    // Special
    END_OF_FILE,
    UNKNOWN
};

struct SourceLocation {
    int line;
    int column;
};

struct Token {
    TokenType type;
    std::string lexeme;
    SourceLocation location;

    Token(TokenType type, std::string lexeme, SourceLocation loc)
        : type(type), lexeme(std::move(lexeme)), location(loc) {}
};

#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Token.hpp"
#include "LexerError.hpp"

// Lexer: Converts raw source text into a stream of Tokens.
// Handles location tracking, escape characters, and reports lexical errors.
class Lexer {
public:
    // Construct a lexer from source string
    Lexer(const std::string& input);

    // Return the next token in the input stream
    std::shared_ptr<Token> nextToken();

    // Get all errors found during lexing
    const std::vector<LexerError>& getErrors() const;

private:
    std::string source;
    size_t pos;
    int line;
    int column;
    std::vector<LexerError> errors;

    // Read next character without advancing position
    char peek() const;

    // Consume the current character and advance position
    char get();

    // Advance multiple positions
    void advance(int count);

    // Skip whitespace characters (space, tab, newline)
    void skipWhitespace();

    // Match identifiers or keywords
    std::shared_ptr<Token> identifierOrKeyword();

    // Match numeric literals
    std::shared_ptr<Token> number();

    // Match string literals, preserving escape sequences
    std::shared_ptr<Token> stringLiteral();

    // Match all operators and punctuation
    std::shared_ptr<Token> matchOperator();

    // Character class utilities
    bool isAlpha(char c) const;
    bool isDigit(char c) const;
    bool isAlnum(char c) const;

    // Add error with position and offending lexeme
    void reportError(const std::string& msg, const std::string& offending);
};

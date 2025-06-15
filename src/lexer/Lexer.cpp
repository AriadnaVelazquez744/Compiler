#include "Lexer.hpp"
#include <cctype>
#include <iostream>
#include <unordered_map>

Lexer::Lexer(const std::string& input)
    : source(input), pos(0), line(1), column(1) {}

char Lexer::peek() const {
    return (pos < source.size()) ? source[pos] : '\0';
}

char Lexer::get() {
    char c = peek();
    if (c == '\n') {
        ++line;
        column = 1;
    } else {
        ++column;
    }
    ++pos;
    return c;
}

void Lexer::advance(int count) {
    for (int i = 0; i < count; ++i) get();
}

void Lexer::skipWhitespace() {
    while (isspace(peek())) get();
}

bool Lexer::isAlpha(char c) const {
    return std::isalpha(c) || c == '_';
}

bool Lexer::isDigit(char c) const {
    return std::isdigit(c);
}

bool Lexer::isAlnum(char c) const {
    return std::isalnum(c) || c == '_';
}

void Lexer::reportError(const std::string& msg, const std::string& offending) {
    errors.emplace_back(msg, SourceLocation{line, column}, offending);
}

std::shared_ptr<Token> Lexer::identifierOrKeyword() {
    size_t start = pos;
    SourceLocation loc = {line, column};

    while (isAlnum(peek())) get();

    std::string text = source.substr(start, pos - start);

    static const std::unordered_map<std::string, TokenType> keywords = {
        {"let", TokenType::LET}, {"in", TokenType::IN},
        {"True", TokenType::BOOL}, {"False", TokenType::BOOL},
        {"Null", TokenType::NULL_VAL}, {"print", TokenType::PRINT},
        {"read", TokenType::READ}, {"parse", TokenType::PARSE},
        {"if", TokenType::IF}, {"else", TokenType::ELSE},
        {"elif", TokenType::ELIF}, {"for", TokenType::FOR},
        {"while", TokenType::WHILE}, {"range", TokenType::RANGE},
        {"function", TokenType::FUNC}, {"type", TokenType::TYPE},
        {"new", TokenType::NEW}, {"self", TokenType::SELF},
        {"inherits", TokenType::INHERITS}, {"base", TokenType::BASE},
        {"sin", TokenType::SIN}, {"cos", TokenType::COS},
        {"max", TokenType::MAX}, {"min", TokenType::MIN},
        {"sqrt", TokenType::SQRT}, {"exp", TokenType::EXP},
        {"log", TokenType::LOG}, {"rand", TokenType::RANDOM},
        {"PI", TokenType::PI}, {"E", TokenType::E}
    };

    auto it = keywords.find(text);
    if (it != keywords.end()) {
        return std::make_shared<Token>(it->second, text, loc);
    }

    return std::make_shared<Token>(TokenType::ID, text, loc);
}

std::shared_ptr<Token> Lexer::number() {
    size_t start = pos;
    SourceLocation loc = {line, column};

    while (isdigit(peek())) get();
    if (peek() == '.') {
        get();
        while (isdigit(peek())) get();
    }

    std::string lexeme = source.substr(start, pos - start);
    return std::make_shared<Token>(TokenType::NUMBER, lexeme, loc);
}

std::shared_ptr<Token> Lexer::stringLiteral() {
    SourceLocation loc = {line, column};
    std::string result;

    get();  // consume opening "

    while (peek() != '"' && peek() != '\0') {
        char c = get();
        if (c == '\\') {
            result += '\\';
            result += get();  // keep escaped character as-is
        } else {
            result += c;
        }
    }

    if (peek() == '"') {
        get(); // consume closing "
        return std::make_shared<Token>(TokenType::STRING, result, loc);
    }

    reportError("Unterminated string literal", result);
    return std::make_shared<Token>(TokenType::UNKNOWN, result, loc);
}

std::shared_ptr<Token> Lexer::matchOperator() {
    SourceLocation loc = {line, column};
    char c = peek();

    auto match = [&](std::string op, TokenType type) -> std::shared_ptr<Token> {
        advance(op.length());
        return std::make_shared<Token>(type, op, loc);
    };

    std::string next2 = source.substr(pos, 2);

    // Multi-char operators
    if (next2 == "**") return match("**", TokenType::POW);
    if (next2 == "@@") return match("@@", TokenType::CONCAT_SPACE);
    if (next2 == "=>") return match("=>", TokenType::LAMBDA);
    if (next2 == ":=") return match(":=", TokenType::REASSIGN);
    if (next2 == "==") return match("==", TokenType::EQ);
    if (next2 == "!=") return match("!=", TokenType::NE);
    if (next2 == "<=") return match("<=", TokenType::LE);
    if (next2 == ">=") return match(">=", TokenType::GE);

    // Single-char
    switch (c) {
        case '+': return match("+", TokenType::ADD);
        case '-': return match("-", TokenType::SUB);
        case '*': return match("*", TokenType::MUL);
        case '/': return match("/", TokenType::DIV);
        case '%': return match("%", TokenType::MOD);
        case '^': return match("^", TokenType::POW);
        case '@': return match("@", TokenType::CONCAT);
        case '&': return match("&", TokenType::AND);
        case '|': return match("|", TokenType::OR);
        case '!': return match("!", TokenType::NOT);
        case '<': return match("<", TokenType::LT);
        case '>': return match(">", TokenType::GT);
        case '=': return match("=", TokenType::ASSIGN);
        case ',': return match(",", TokenType::COMMA);
        case ';': return match(";", TokenType::SEMICOLON);
        case '.': return match(".", TokenType::DOT);
        case '(': return match("(", TokenType::LPAREN);
        case ')': return match(")", TokenType::RPAREN);
        case '{': return match("{", TokenType::LBRACE);
        case '}': return match("}", TokenType::RBRACE);
        case ':': return match(":", TokenType::DEFINE);
        default: break;
    }

    reportError("Unrecognized symbol", std::string(1, c));
    get();
    return std::make_shared<Token>(TokenType::UNKNOWN, std::string(1, c), loc);
}

std::shared_ptr<Token> Lexer::nextToken() {
    skipWhitespace();

    if (pos >= source.size())
        return std::make_shared<Token>(TokenType::END_OF_FILE, "", SourceLocation{line, column});

    char c = peek();

    if (isAlpha(c)) return identifierOrKeyword();
    if (isdigit(c)) return number();
    if (c == '"') return stringLiteral();

    return matchOperator();

}

const std::vector<LexerError>& Lexer::getErrors() const {
    return errors;
}

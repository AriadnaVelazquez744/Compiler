#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "lexer/LexerError.hpp"

// Utility to convert TokenType enum to string (for printing)
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::STRING: return "STRING";
        case TokenType::BOOL: return "BOOL";
        case TokenType::NULL_VAL: return "NULL_VAL";
        case TokenType::ID: return "ID";
        case TokenType::ADD: return "ADD";
        case TokenType::SUB: return "SUB";
        case TokenType::MUL: return "MUL";
        case TokenType::DIV: return "DIV";
        case TokenType::MOD: return "MOD";
        case TokenType::POW: return "POW";
        case TokenType::LT: return "LT";
        case TokenType::GT: return "GT";
        case TokenType::LE: return "LE";
        case TokenType::GE: return "GE";
        case TokenType::EQ: return "EQ";
        case TokenType::NE: return "NE";
        case TokenType::AND: return "AND";
        case TokenType::OR: return "OR";
        case TokenType::NOT: return "NOT";
        case TokenType::CONCAT: return "CONCAT";
        case TokenType::CONCAT_SPACE: return "CONCAT_SPACE";
        case TokenType::SIN: return "SIN";
        case TokenType::COS: return "COS";
        case TokenType::MAX: return "MAX";
        case TokenType::MIN: return "MIN";
        case TokenType::SQRT: return "SQRT";
        case TokenType::EXP: return "EXP";
        case TokenType::LOG: return "LOG";
        case TokenType::RANDOM: return "RANDOM";
        case TokenType::PRINT: return "PRINT";
        case TokenType::READ: return "READ";
        case TokenType::PARSE: return "PARSE";
        case TokenType::PI: return "PI";
        case TokenType::E: return "E";
        case TokenType::FUNC: return "FUNC";
        case TokenType::LET: return "LET";
        case TokenType::IN: return "IN";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::ELIF: return "ELIF";
        case TokenType::FOR: return "FOR";
        case TokenType::WHILE: return "WHILE";
        case TokenType::RANGE: return "RANGE";
        case TokenType::TYPE: return "TYPE";
        case TokenType::NEW: return "NEW";
        case TokenType::SELF: return "SELF";
        case TokenType::INHERITS: return "INHERITS";
        case TokenType::COMMA: return "COMMA";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::DOT: return "DOT";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::REASSIGN: return "REASSIGN";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::UNKNOWN: return "UNKNOWN";
        default: return "UNDEFINED";
    }
}

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char** argv) {
    std::string filename = "script.hulk";

    if (argc >= 2) {
        filename = argv[1];
    }

    std::string sourceCode;
    try {
        sourceCode = readFile(filename);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    Lexer lexer(sourceCode);

    std::cout << "== Tokens leídos del archivo '" << filename << "':\n\n";

    while (true) {
        auto token = lexer.nextToken();

        if (token->type == TokenType::END_OF_FILE) break;

        std::cout << "[Línea " << token->location.line
                  << ", Columna " << token->location.column << "] "
                  << tokenTypeToString(token->type)
                  << " → \"" << token->lexeme << "\"\n";
    }

    const auto& errors = lexer.getErrors();
    if (!errors.empty()) {
        std::cerr << "\n== Errores léxicos encontrados:\n";
        for (const auto& err : errors) {
            std::cerr << "[Línea " << err.location.line
                      << ", Columna " << err.location.column << "] "
                      << "Error: " << err.message
                      << " → \"" << err.offendingLexeme << "\"\n";
        }
        return 1;
    }

    std::cout << "\n== Análisis léxico completado sin errores.\n";
    return 0;
}

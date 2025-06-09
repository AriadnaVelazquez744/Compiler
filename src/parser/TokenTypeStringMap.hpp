#pragma once
#include "../lexer/Lexer.hpp"
#include <string>
#include <unordered_map>

inline std::string tokenTypeToString(TokenType type) {
    static const std::unordered_map<TokenType, std::string> mapping = {
        {TokenType::ADD, "ADD"}, {TokenType::SUB, "SUB"}, {TokenType::MUL, "MUL"},
        {TokenType::DIV, "DIV"}, {TokenType::MOD, "MOD"}, {TokenType::POW, "POW"},
        {TokenType::AND, "AND"}, {TokenType::OR, "OR"}, {TokenType::NOT, "NOT"},
        {TokenType::LT, "LT"}, {TokenType::GT, "GT"}, {TokenType::LE, "LE"},
        {TokenType::GE, "GE"}, {TokenType::EQ, "EQ"}, {TokenType::NE, "NE"},
        {TokenType::CONCAT, "CONCAT"}, {TokenType::CONCAT_SPACE, "CONCAT_SPACE"},
        {TokenType::SIN, "SIN"}, {TokenType::COS, "COS"}, {TokenType::MAX, "MAX"},
        {TokenType::MIN, "MIN"}, {TokenType::SQRT, "SQRT"}, {TokenType::EXP, "EXP"},
        {TokenType::LOG, "LOG"}, {TokenType::RANDOM, "RANDOM"},
        {TokenType::PI, "PI"}, {TokenType::E, "E"},
        {TokenType::LPAREN, "LPAREN"}, {TokenType::RPAREN, "RPAREN"},
        {TokenType::LBRACE, "LBRACE"}, {TokenType::RBRACE, "RBRACE"},
        {TokenType::COMMA, "COMMA"}, {TokenType::SEMICOLON, "SEMICOLON"}, 
        {TokenType::ASSIGN, "ASSIGN"}, {TokenType::REASSIGN, "REASSIGN"},
        {TokenType::IF, "IF"}, {TokenType::ELSE, "ELSE"}, {TokenType::WHILE, "WHILE"},
        {TokenType::FOR, "FOR"}, {TokenType::RANGE, "RANGE"},
        {TokenType::FUNC, "FUNC"}, {TokenType::LAMBDA, "LAMBDA"}, 
        {TokenType::LET, "LET"}, {TokenType::IN, "IN"},
        {TokenType::NUMBER, "NUMBER"}, {TokenType::STRING, "STRING"},
        {TokenType::ID, "ID"}, {TokenType::BOOL, "BOOL"}, {TokenType::NULL_VAL, "NULL_VAL"}, 
        {TokenType::PRINT, "PRINT"}, {TokenType::READ, "READ"}, {TokenType::PARSE, "PARSE"},
        {TokenType::ELIF, "ELIF"}, {TokenType::TYPE, "TYPE"}, {TokenType::NEW, "NEW"},
        {TokenType::SELF, "SELF"}, {TokenType::INHERITS, "INHERITS"},
        {TokenType::DOT, "DOT"},
        {TokenType::END_OF_FILE, "$"},
    };
    auto it = mapping.find(type);
    return (it != mapping.end()) ? it->second : "UNKNOWN";
}

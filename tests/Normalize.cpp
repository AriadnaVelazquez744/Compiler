#include "../src/parser/GrammarNormalizer.hpp"

int main() {
    std::vector<RawRule> rules = {
        {"S`", "program"},
        {"program","(stm | (error SEMICOLON))* "},
        {"stm", "(((func_decl | print | expr) SEMICOLON) | (cond (SEMICOLON)?))"},

        {"print", "PRINT LPAREN value RPAREN"},

        {"func_decl", "FUNC ID LPAREN (params)? RPAREN ((LAMBDA value) | block_expr)"},
        {"params", "ID (COMMA ID)*"},

        {"expr", "NUMBER | STRING | BOOLEAN | NULL_VAL | ID | bin_expr | unary_expr | brace | func_call_expr | reassign_expr | const | build_in"},

        {"value", "exp | cond"},
        {"arg", "value (COMMA value)*"},

        {"reassign_expr", "ID REASSIGN value"},
        {"func_call_expr", "ID LPAREN (arg)? RPAREN"},
        {"const", "E | PI"},
        {"bin_expr", "value (ADD | SUB | MULT | DIV | MOD | POW | CONCAT | CONCAT_SPACE | LT | GT | LE | GE | EQ | NE | AND | OR) value"},
        {"unary_expr", "(SUB | NOT) value"},
        {"brace", "LPAREN value RPAREN"},
        {"build_in", "(SIN | COS | MIN | MAX | SQRT | EXP | RANDOM | LOG) LPAREN (arg)? RPAREN"},

        {"cond", "if_expr | while_expr | for_expr | let_expr | block_expr"},

        {"block_expr", "LBRACE ((stm | (cond SEMICOLON))* | body) RBRACE"},

        {"body", "stm | print | func_decl | value"},

        {"if_expr", "IF LPAREN value RPAREN body (ELIF LPAREN value RPAREN body)* ELSE body"},
        {"let_expr", "LET ID ASSIGN value (COMMA ID ASSIGN value)* IN ((LPAREN body RPAREN) | body) (SEMICOLON)? "},
        {"while_expr", "WHILE LPAREN value RPAREN body"},
        {"for_expr", "FOR LPAREN ID IN RANGE LPAREN value COMMA value RPAREN RPAREN body"},

    };

    GrammarNormalizer normalizer;
    normalizer.normalizeRulesToFile(rules, "src/parser/BNFGrammar.bnf");

    return 0;
}

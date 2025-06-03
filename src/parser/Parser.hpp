//Parser.hpp
#pragma once

#include "../lexer/Lexer.hpp"
#include "LR1ParsingTables.hpp"
#include "SemanticActionProvider.hpp"
#include <vector>
#include <memory>

class Parser {
public:
    Parser(Lexer& lexer,
           const LR1ParsingTableGenerator& parsingTables,
           const GrammarAugment& grammar,
           std::shared_ptr<SemanticActionProvider> actionProvider);

    std::vector<std::shared_ptr<ASTNode>> parse();  // Return multiple statements

    std::string tokenName = getTerminalName(token->type);

private:
    Lexer& lexer;
    const LR1ParsingTableGenerator& parsingTables;
    const GrammarAugment& grammar;
    std::shared_ptr<SemanticActionProvider> actionProvider;

    std::vector<std::pair<std::string, std::vector<std::string>>> flatProductions;
    void buildFlatProductionList();
};

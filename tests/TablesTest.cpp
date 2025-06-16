#include "../src/parser/core/GrammarAugment.hpp"
#include "../src/parser/core/LR1ItemSetBuilder.hpp"
#include "../src/parser/core/LR1ParsingTables.hpp"
#include "../src/parser/grammar/PrecedenceSetup.hpp"
#include <iostream>

int main() {
    GrammarAugment grammar;
    grammar.readGrammar("src/parser/grammar/BNFGrammar.bnf");
    grammar.computeFirstSets();
    grammar.computeFollowSets();

    std::cout << "\n Construcción de conjuntos FIRST y FALLOW" << std::endl;
    // std::cout << "=== FIRST Sets ===" << std::endl;
    grammar.printFirstSets();

    // std::cout << "\n=== FOLLOW Sets ===" << std::endl;
    grammar.printFollowSets();

    LR1ItemSetBuilder builder(grammar);
    builder.constructItemSets();
    builder.printItemSets();
    std::cout << "\n Construcción de los conjuntos LR1" << std::endl;

    LR1ParsingTableGenerator tableBuilder(grammar, builder);

    
    setupPrecedence(tableBuilder); // External config
    tableBuilder.generateParsingTables();
    tableBuilder.printActionTable();
    tableBuilder.printGotoTable();
    tableBuilder.printExpectedTokens();
    std::cout << "\n Se generaron las tablas ACTION y GOTO" << std::endl;

    return 0;
}

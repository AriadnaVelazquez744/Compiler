#include "../src/parser/GrammarAugment.hpp"
#include "../src/parser/LR1ItemSetBuilder.hpp"
#include "../src/parser/LR1ParsingTables.hpp"
#include "../src/parser/PrecedenceSetup.hpp"
#include <iostream>

int main() {
    GrammarAugment grammar;
    grammar.readGrammar("src/parser/BNFGrammar.bnf");
    grammar.computeFirstSets();
    grammar.computeFollowSets();

    std::cout << "\n Construcción de conjuntos FIRST y FALLOW" << std::endl;
    // std::cout << "=== FIRST Sets ===" << std::endl;
    // grammar.printFirstSets();

    // std::cout << "\n=== FOLLOW Sets ===" << std::endl;
    // grammar.printFollowSets();

    LR1ItemSetBuilder builder(grammar);
    builder.constructItemSets();
    // builder.printItemSets();
    std::cout << "\n Construcción de los conjuntos LR1" << std::endl;

    LR1ParsingTableGenerator tableBuilder(grammar, builder);

    
    setupPrecedence(tableBuilder); // External config
    tableBuilder.generateParsingTables();
    // tableBuilder.printActionTable();
    std::cout << "\n Se generaron las tablas ACTION y GOTO" << std::endl;

    return 0;
}

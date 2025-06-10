#include "../src/parser/GrammarAugment.hpp"
#include "../src/parser/LR1ItemSetBuilder.hpp"
#include <iostream>

int main() {
    GrammarAugment grammar;
    grammar.readGrammar("src/parser/BNFGrammar.bnf");

    std::cout << "=== FIRST Sets ===\n";
    grammar.computeFirstSets();
    grammar.printFirstSets();

    std::cout << "\n=== FOLLOW Sets ===\n";
    grammar.computeFollowSets();
    grammar.printFollowSets();

    std::cout << "\n=== LR(1) Item Sets ===\n";
    LR1ItemSetBuilder builder(grammar);
    builder.constructItemSets();
    builder.printItemSets();

    return 0;
}

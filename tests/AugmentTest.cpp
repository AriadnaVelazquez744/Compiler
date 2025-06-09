#include "../src/parser/GrammarAugment.hpp"
#include <iostream>

int main() {
    GrammarAugment grammar;
    grammar.readGrammar("src/parser/BNFGrammar.bnf");
    grammar.computeFirstSets();
    grammar.computeFollowSets();
    grammar.printFirstSets();
    grammar.printFollowSets();
    return 0;
}
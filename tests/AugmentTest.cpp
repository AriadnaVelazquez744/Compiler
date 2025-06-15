#include "../src/parser/grammar/GrammarAugment.hpp"
#include <iostream>

int main() {
    GrammarAugment grammar;
    grammar.readGrammar("src/parser/grammar/BNFGrammar.bnf");
    grammar.computeFirstSets();
    grammar.computeFollowSets();
    grammar.printFirstSets();
    grammar.printFollowSets();
    return 0;
}
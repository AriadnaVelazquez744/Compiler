//GrammarAugment.hpp
#pragma once
#include <vector>
#include <map>
#include <set>
#include <string>

class GrammarAugment {
public:
    void readGrammar(const std::string& filename);
    void computeFirstSets();
    void computeFollowSets();
    void printFirstSets() const;
    void printFollowSets() const;
    std::set<std::string> computeFirstForSequence(const std::vector<std::string>& seq) const;
    const std::string& getStartSymbol() const { return startSymbol; }
    
    const std::map<std::string, std::vector<std::vector<std::string>>>& getProductions() const { return productions; }
    const std::map<std::pair<std::string, std::vector<std::string>>, int>& getProductionToIndex() const { return productionToIndex; }

    bool isNonTerminal(const std::string& sym) const { return nonTerminals.count(sym); }
    bool isTerminal(const std::string& sym) const { return terminals.count(sym); }

    // Add method to augment grammar
    void augmentGrammar();

private:

    std::map<std::string, std::vector<std::vector<std::string>>> productions;
    std::map<std::pair<std::string, std::vector<std::string>>, int> productionToIndex;

    std::set<std::string> nonTerminals;
    std::set<std::string> terminals;
    std::string startSymbol;

    std::map<std::string, std::set<std::string>> firstSet;
    std::map<std::string, std::set<std::string>> followSet;
};
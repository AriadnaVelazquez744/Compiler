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
    bool isNonTerminal(const std::string& sym) const { return nonTerminals.count(sym); }

private:

    std::map<std::string, std::vector<std::vector<std::string>>> productions;
    std::set<std::string> nonTerminals;
    std::set<std::string> terminals;
    std::string startSymbol;

    std::map<std::string, std::set<std::string>> firstSet;
    std::map<std::string, std::set<std::string>> followSet;
};
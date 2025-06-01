//GrammarAugment.cpp
#include "GrammarAugment.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

void GrammarAugment::readGrammar(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return;
    }

    std::string line;
    bool isFirstRule = true;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        
        while (iss >> token) tokens.push_back(token);
        if (tokens.size() < 2 || tokens[1] != ":") continue;

        std::string nonTerminal = tokens[0];
        std::vector<std::string> production;
        for (size_t i = 2; i < tokens.size(); ++i) {
            production.push_back(tokens[i]);
        }

        productions[nonTerminal].push_back(production);
        nonTerminals.insert(nonTerminal);
        if (isFirstRule) {
            startSymbol = nonTerminal;
            isFirstRule = false;
        }
    }

    // Collect terminals (symbols not in nonTerminals)
    for (const auto& prodPair : productions) {
        for (const auto& prod : prodPair.second) {
            for (const std::string& symbol : prod) {
                if (nonTerminals.find(symbol) == nonTerminals.end() && symbol != "ε") {
                    terminals.insert(symbol);
                }
            }
        }
    }

    // Initialize FIRST sets for terminals
    for (const std::string& t : terminals) {
        firstSet[t] = {t};
    }
    for (const std::string& nt : nonTerminals) {
        firstSet[nt] = {};
    }
}

std::set<std::string> GrammarAugment::computeFirstForSequence(const std::vector<std::string>& seq) {
    if (seq.empty()) return {"ε"};

    std::set<std::string> result;
    bool allDeriveEpsilon = true;

    for (const std::string& symbol : seq) {
        if (terminals.find(symbol) != terminals.end()) {
            result.insert(symbol);
            allDeriveEpsilon = false;
            break;
        } else if (nonTerminals.find(symbol) != nonTerminals.end()) {
            const auto& first = firstSet[symbol];
            bool hasEpsilon = false;
            for (const std::string& s : first) {
                if (s == "ε") hasEpsilon = true;
                else result.insert(s);
            }
            if (!hasEpsilon) {
                allDeriveEpsilon = false;
                break;
            }
        } else {
            result.insert(symbol);
            allDeriveEpsilon = false;
            break;
        }
    }

    if (allDeriveEpsilon) result.insert("ε");
    return result;
}

void GrammarAugment::computeFirstSets() {
    bool changed = true;
    while (changed) {
        changed = false;
        for (const std::string& nt : nonTerminals) {
            for (const auto& prod : productions[nt]) {
                std::set<std::string> firstBody = computeFirstForSequence(prod);
                for (const std::string& sym : firstBody) {
                    if (firstSet[nt].find(sym) == firstSet[nt].end()) {
                        firstSet[nt].insert(sym);
                        changed = true;
                    }
                }
            }
        }
    }
}

void GrammarAugment::computeFollowSets() {
    for (const std::string& nt : nonTerminals) followSet[nt] = {};
    followSet[startSymbol] = {"$"};

    bool changed = true;
    while (changed) {
        changed = false;
        for (const std::string& A : nonTerminals) {
            for (const auto& prod : productions[A]) {
                for (size_t i = 0; i < prod.size(); ++i) {
                    const std::string& B = prod[i];
                    if (nonTerminals.find(B) == nonTerminals.end()) continue;

                    std::vector<std::string> beta(prod.begin() + i + 1, prod.end());
                    std::set<std::string> firstBeta = computeFirstForSequence(beta);

                    // Add non-epsilon symbols from FIRST(β)
                    for (const std::string& sym : firstBeta) {
                        if (sym != "ε" && followSet[B].find(sym) == followSet[B].end()) {
                            followSet[B].insert(sym);
                            changed = true;
                        }
                    }

                    // If β derives ε, add FOLLOW(A) to FOLLOW(B)
                    if (firstBeta.find("ε") != firstBeta.end()) {
                        for (const std::string& sym : followSet[A]) {
                            if (followSet[B].find(sym) == followSet[B].end()) {
                                followSet[B].insert(sym);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
    }
}

void GrammarAugment::printFirstSets() const {
    std::cout << "FIRST Sets:\n";
    for (const std::string& nt : nonTerminals) {
        std::cout << nt << " = { ";
        for (const std::string& s : firstSet.at(nt)) {
            std::cout << s << " ";
        }
        std::cout << "}\n";
    }
}

void GrammarAugment::printFollowSets() const {
    std::cout << "FOLLOW Sets:\n";
    for (const std::string& nt : nonTerminals) {
        std::cout << nt << " = { ";
        for (const std::string& s : followSet.at(nt)) {
            std::cout << s << " ";
        }
        std::cout << "}\n";
    }
}
//LR1ItemSetBuilder.cpp
#include "LR1ItemSetBuilder.hpp"
#include <iostream>
#include <algorithm>

LR1ItemSetBuilder::LR1ItemSetBuilder(const GrammarAugment& g) : grammar(g) {}

void LR1ItemSetBuilder::constructItemSets() {
    std::string start = grammar.getStartSymbol();
    std::string augmented = start + "'";
    std::set<LR1Item> startItemSet = closure({{augmented, {start}, 0, "$"}});
    itemSets.push_back(startItemSet);
    transitions.push_back({});

    for (size_t i = 0; i < itemSets.size(); ++i) {
        std::map<std::string, std::set<LR1Item>> nextSets;

        for (const auto& item : itemSets[i]) {
            if (item.dotPos < item.rhs.size()) {
                std::string nextSym = item.rhs[item.dotPos];
                auto nextItem = item;
                ++nextItem.dotPos;
                nextSets[nextSym].insert(nextItem);
            }
        }

        for (const auto& [sym, items] : nextSets) {
            std::set<LR1Item> closureSet = closure(items);
            auto it = std::find(itemSets.begin(), itemSets.end(), closureSet);
            size_t idx;
            if (it == itemSets.end()) {
                itemSets.push_back(closureSet);
                transitions.push_back({}); // <-- add this line
                idx = itemSets.size() - 1;
            } else {
                idx = std::distance(itemSets.begin(), it);
            }
            transitions[i][sym] = idx;
        }
    }
}

std::set<std::string> LR1ItemSetBuilder::firstOfSequence(const std::vector<std::string>& seq) const {
    return grammar.computeFirstForSequence(seq);
}

std::set<LR1Item> LR1ItemSetBuilder::closure(const std::set<LR1Item>& items) const {
    std::set<LR1Item> closureSet = items;
    bool changed = true;

    while (changed) {
        changed = false;
        std::set<LR1Item> newItems;

        for (const auto& item : closureSet) {
            if (item.dotPos >= item.rhs.size()) continue;

            const std::string& B = item.rhs[item.dotPos];
            if (!grammar.isNonTerminal(B)) continue;

            std::vector<std::string> beta(item.rhs.begin() + item.dotPos + 1, item.rhs.end());
            beta.push_back(item.lookahead);
            std::set<std::string> lookaheadSet = firstOfSequence(beta);

            const auto& Bprods = grammar.getProductions().at(B);
            for (const auto& rhs : Bprods) {
                for (const std::string& la : lookaheadSet) {
                    LR1Item newItem{B, rhs, 0, la};
                    if (closureSet.find(newItem) == closureSet.end()) {
                        newItems.insert(newItem);
                        changed = true;
                    }
                }
            }
        }

        closureSet.insert(newItems.begin(), newItems.end());
    }

    return closureSet;
}

void LR1ItemSetBuilder::printItemSets() const {
    for (size_t i = 0; i < itemSets.size(); ++i) {
        std::cout << "Item Set " << i << ":\n";
        for (const auto& item : itemSets[i]) {
            std::cout << "  " << item.lhs << " → ";
            for (size_t j = 0; j < item.rhs.size(); ++j) {
                if (j == item.dotPos) std::cout << "• ";
                std::cout << item.rhs[j] << " ";
            }
            if (item.dotPos == item.rhs.size()) std::cout << "• ";
            std::cout << ", " << item.lookahead << "\n";
        }
        std::cout << std::endl;
    }
}

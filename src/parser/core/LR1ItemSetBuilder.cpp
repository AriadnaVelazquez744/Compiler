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

    std::unordered_map<std::set<LR1Item>, size_t> itemSetToIndex;
    itemSetToIndex[startItemSet] = 0;

    for (size_t i = 0; i < itemSets.size(); ++i) {
        std::map<std::string, std::set<LR1Item>> nextSets;

        for (const auto& item : itemSets[i]) {
            if (item.dotPos < item.rhs.size()) {
                std::string sym = item.rhs[item.dotPos];
                LR1Item nextItem = item;
                nextItem.dotPos++;
                nextSets[sym].insert(nextItem);
            }
        }

        for (const auto& [sym, kernelItems] : nextSets) {
            std::set<LR1Item> closureSet = closure(kernelItems);

            auto [it, inserted] = itemSetToIndex.emplace(closureSet, itemSets.size());
            if (inserted) {
                itemSets.push_back(closureSet);
                transitions.push_back({});
            }
            transitions[i][sym] = it->second;
        }
    }
}

std::set<std::string> LR1ItemSetBuilder::firstOfSequence(const std::vector<std::string>& seq) const {
    return grammar.computeFirstForSequence(seq);
}

std::set<LR1Item> LR1ItemSetBuilder::closure(const std::set<LR1Item>& items) const {
    // Check full cache first
    auto cacheIt = closureCache.find(items);
    if (cacheIt != closureCache.end()) return cacheIt->second;

    std::set<LR1Item> closureSet = items;
    std::vector<LR1Item> worklist(items.begin(), items.end());

    while (!worklist.empty()) {
        LR1Item item = worklist.back();
        worklist.pop_back();

        if (item.dotPos >= item.rhs.size()) continue;

        const std::string& B = item.rhs[item.dotPos];
        if (!grammar.isNonTerminal(B)) continue;

        std::vector<std::string> beta(item.rhs.begin() + item.dotPos + 1, item.rhs.end());
        beta.push_back(item.lookahead);

        // Cache FIRST(βa)
        static std::unordered_map<std::vector<std::string>, std::set<std::string>> firstCache;
        auto itFirst = firstCache.find(beta);
        std::set<std::string> lookaheads;
        if (itFirst != firstCache.end()) {
            lookaheads = itFirst->second;
        } else {
            lookaheads = firstOfSequence(beta);
            firstCache[beta] = lookaheads;
        }

        const auto& Bprods = grammar.getProductions().at(B);
        for (const auto& rhs : Bprods) {
            for (const auto& la : lookaheads) {
                LR1Item newItem{B, rhs, 0, la};
                if (closureSet.insert(newItem).second) {
                    worklist.push_back(newItem); // new item added
                }
            }
        }
    }

    closureCache[items] = closureSet;
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

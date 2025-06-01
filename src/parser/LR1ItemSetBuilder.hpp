//LR1ItemSetBuilder.hpp
#pragma once

#include "GrammarAugment.hpp"
#include <string>
#include <set>
#include <map>
#include <vector>

struct LR1Item {
    std::string lhs;
    std::vector<std::string> rhs;
    size_t dotPos;
    std::string lookahead;

    bool operator<(const LR1Item& other) const {
        return std::tie(lhs, rhs, dotPos, lookahead) < std::tie(other.lhs, other.rhs, other.dotPos, other.lookahead);
    }
    bool operator==(const LR1Item& other) const {
        return lhs == other.lhs &&
               rhs == other.rhs &&
               dotPos == other.dotPos &&
               lookahead == other.lookahead;
    }
};

class LR1ItemSetBuilder {
public:
    explicit LR1ItemSetBuilder(const GrammarAugment& grammar);
    void constructItemSets();
    void printItemSets() const;

private:
    std::set<LR1Item> closure(const std::set<LR1Item>& items) const;
    std::set<std::string> firstOfSequence(const std::vector<std::string>& seq) const;

    const GrammarAugment& grammar;
    std::vector<std::set<LR1Item>> itemSets;
    std::vector<std::map<std::string, size_t>> transitions;
};

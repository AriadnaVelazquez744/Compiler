//LR1ItemSetBuilder.hpp
#pragma once

#include "GrammarAugment.hpp"
#include <string>
#include <set>
#include <map>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <functional>

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

struct LR1ItemCore {
    std::string lhs;
    std::vector<std::string> rhs;
    size_t dotPos;

    bool operator<(const LR1ItemCore& other) const {
        if (lhs != other.lhs) return lhs < other.lhs;
        if (dotPos != other.dotPos) return dotPos < other.dotPos;
        return rhs < other.rhs;
    }

    bool operator==(const LR1ItemCore& other) const {
        return lhs == other.lhs &&
               rhs == other.rhs &&
               dotPos == other.dotPos;
    }
};

namespace std {
    template<>
    struct hash<LR1ItemCore> {
        size_t operator()(const LR1ItemCore& item) const {
            size_t h1 = hash<string>()(item.lhs);
            size_t h2 = 0;
            for (const auto& s : item.rhs) h2 ^= hash<string>()(s) + 0x9e3779b9 + (h2<<6) + (h2>>2);
            size_t h3 = hash<size_t>()(item.dotPos);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
    template<>
    struct hash<LR1Item> {
        size_t operator()(const LR1Item& item) const {
            size_t h1 = hash<string>()(item.lhs);
            size_t h2 = 0;
            for (const auto& s : item.rhs) h2 ^= hash<string>()(s) + 0x9e3779b9 + (h2<<6) + (h2>>2);
            size_t h3 = hash<size_t>()(item.dotPos);
            size_t h4 = hash<string>()(item.lookahead);
            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
        }
    };
    template<>
    struct hash<std::set<LR1Item>> {
        size_t operator()(const std::set<LR1Item>& s) const {
            size_t h = 0;
            for (const auto& item : s) {
                h ^= std::hash<LR1Item>()(item) + 0x9e3779b9 + (h << 6) + (h >> 2);
            }
            return h;
        }
    };

    template<>
    struct hash<std::vector<std::string>> {
        std::size_t operator()(const std::vector<std::string>& vec) const {
            std::size_t seed = 0;
            for (const auto& s : vec) {
                seed ^= std::hash<std::string>()(s) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}

class LR1ItemSetBuilder {
public:
    explicit LR1ItemSetBuilder(const GrammarAugment& grammar);
    void constructItemSets();
    void printItemSets() const;
    const std::vector<std::set<LR1Item>>& getItemSets() const { return itemSets; }
    const std::vector<std::map<std::string, size_t>>& getTransitions() const { return transitions; }

private:
    std::set<LR1Item> closure(const std::set<LR1Item>& items) const;
    std::set<std::string> firstOfSequence(const std::vector<std::string>& seq) const;

    const GrammarAugment& grammar;
    std::vector<std::set<LR1Item>> itemSets;
    std::vector<std::map<std::string, size_t>> transitions;
    // Optimized closure cache: per LR1Item set
    mutable std::unordered_map<std::set<LR1Item>, std::set<LR1Item>> closureCache;
};

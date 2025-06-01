//LR1ParsingTables.hpp
#pragma once

#include "GrammarAugment.hpp"
#include "LR1ItemSetBuilder.hpp"
#include <map>
#include <string>
#include <vector>

enum class ActionType {
    Shift,
    Reduce,
    Accept,
    Error
};

struct Action {
    ActionType type;
    int target; // For Shift: next state; For Reduce: production number
};

class LR1ParsingTableGenerator {
public:
    LR1ParsingTableGenerator(const GrammarAugment& grammar,
                             const LR1ItemSetBuilder& itemSetBuilder);

    void generateParsingTables();
    void printActionTable() const;
    void printGotoTable() const;

private:
    const GrammarAugment& grammar;
    const LR1ItemSetBuilder& itemSetBuilder;

    std::vector<std::map<std::string, Action>> actionTable;
    std::vector<std::map<std::string, int>> gotoTable;

    int getProductionNumber(const std::string& lhs, const std::vector<std::string>& rhs) const;
};

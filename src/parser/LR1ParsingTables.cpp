//LR1ParsingTables.cpp
#include "LR1ParsingTables.hpp"
#include <iostream>

LR1ParsingTableGenerator::LR1ParsingTableGenerator(
    const GrammarAugment& grammar,
    const LR1ItemSetBuilder& itemSetBuilder)
    : grammar(grammar), itemSetBuilder(itemSetBuilder) {}

void LR1ParsingTableGenerator::generateParsingTables() {
    const auto& itemSets = itemSetBuilder.getItemSets();
    const auto& transitions = itemSetBuilder.getTransitions();

    actionTable.resize(itemSets.size());
    gotoTable.resize(itemSets.size());

    for (size_t state = 0; state < itemSets.size(); ++state) {
        const auto& items = itemSets[state];

        // Process each item in the current state
        for (const auto& item : items) {
            if (item.dotPos < item.rhs.size()) {
                const std::string& symbol = item.rhs[item.dotPos];
                if (grammar.isTerminal(symbol)) {
                    auto it = transitions[state].find(symbol);
                    if (it != transitions[state].end()) {
                        actionTable[state][symbol] = {ActionType::Shift, static_cast<int>(it->second)};
                    }
                }
            } else {
                if (item.lhs == grammar.getStartSymbol() + "'") {
                    actionTable[state]["$"] = {ActionType::Accept, -1};
                } else {
                    int prodNum = getProductionNumber(item.lhs, item.rhs);
                    actionTable[state][item.lookahead] = {ActionType::Reduce, prodNum};
                }
            }
        }

        // Process GOTO entries for non-terminals
        for (const auto& [symbol, targetState] : transitions[state]) {
            if (grammar.isNonTerminal(symbol)) {
                gotoTable[state][symbol] = static_cast<int>(targetState);
            }
        }
    }
}

void LR1ParsingTableGenerator::printActionTable() const {
    std::cout << "=== ACTION Table ===\n";
    for (size_t state = 0; state < actionTable.size(); ++state) {
        std::cout << "State " << state << ":\n";
        for (const auto& [symbol, action] : actionTable[state]) {
            std::cout << "  [" << symbol << "] = ";
            switch (action.type) {
                case ActionType::Shift:
                    std::cout << "Shift to state " << action.target;
                    break;
                case ActionType::Reduce:
                    std::cout << "Reduce using production " << action.target;
                    break;
                case ActionType::Accept:
                    std::cout << "Accept";
                    break;
                default:
                    std::cout << "Error";
                    break;
            }
            std::cout << "\n";
        }
    }
}

void LR1ParsingTableGenerator::printGotoTable() const {
    std::cout << "=== GOTO Table ===\n";
    for (size_t state = 0; state < gotoTable.size(); ++state) {
        std::cout << "State " << state << ":\n";
        for (const auto& [symbol, targetState] : gotoTable[state]) {
            std::cout << "  [" << symbol << "] = " << targetState << "\n";
        }
    }
}

int LR1ParsingTableGenerator::getProductionNumber(const std::string& lhs,
                                                  const std::vector<std::string>& rhs) const {
    const auto& productions = grammar.getProductions();
    int prodNum = 0;
    for (const auto& [nonTerminal, rhsList] : productions) {
        for (const auto& prodRhs : rhsList) {
            if (nonTerminal == lhs && prodRhs == rhs) {
                return prodNum;
            }
            ++prodNum;
        }
    }
    return -1; // Production not found
}

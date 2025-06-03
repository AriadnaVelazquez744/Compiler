// LR1ParsingTables.hpp
#pragma once

#include "GrammarAugment.hpp"
#include "LR1ItemSetBuilder.hpp"
#include <map>
#include <string>
#include <vector>
#include <set>

enum class ActionType {
    Shift,
    Reduce,
    Accept,
    Error
};

enum class Associativity {
    Left,
    Right,
    NonAssoc
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
    void printExpectedTokens() const;

    // Precedence management
    void setTerminalPrecedence(const std::string& terminal, int precedence, Associativity assoc);
    void setProductionPrecedence(int prodIndex, int precedence);
    int getProductionNumber(const std::string& lhs, const std::vector<std::string>& rhs) const;

    // Error reporting helper
    const std::set<std::string>& getExpectedTokens(int state) const;

    const std::map<std::string, Action>& getActionTable(int state) const {
        return actionTable[state];
    }

    const std::map<std::string, int>& getGotoTable(int state) const {
        return gotoTable[state];
    }


private:
    const GrammarAugment& grammar;
    const LR1ItemSetBuilder& itemSetBuilder;

    std::vector<std::map<std::string, Action>> actionTable;
    std::vector<std::map<std::string, int>> gotoTable;
    std::vector<std::set<std::string>> expectedTokensPerState;

    // Precedence data structures
    std::map<std::string, std::pair<int, Associativity>> terminalPrecedence;
    std::map<int, int> productionPrecedence;
    
    // Conflict resolution
    bool resolveConflict(const Action& existing, const Action& proposed, 
                         const std::string& terminal);
};
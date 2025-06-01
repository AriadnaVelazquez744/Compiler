// LR1ParsingTables.cpp
#include "LR1ParsingTables.hpp"
#include <iostream>
#include <stdexcept>

LR1ParsingTableGenerator::LR1ParsingTableGenerator(
    const GrammarAugment& grammar,
    const LR1ItemSetBuilder& itemSetBuilder)
    : grammar(grammar), itemSetBuilder(itemSetBuilder) {}

void LR1ParsingTableGenerator::setTerminalPrecedence(
    const std::string& terminal, int precedence, Associativity assoc) {
    terminalPrecedence[terminal] = {precedence, assoc};
}

void LR1ParsingTableGenerator::setProductionPrecedence(int prodIndex, int precedence) {
    productionPrecedence[prodIndex] = precedence;
}

const std::set<std::string>& LR1ParsingTableGenerator::getExpectedTokens(int state) const {
    if (state < 0 || state >= expectedTokensPerState.size()) {
        throw std::out_of_range("Invalid state index");
    }
    return expectedTokensPerState[state];
}

bool LR1ParsingTableGenerator::resolveConflict(const Action& existing, 
                                              const Action& proposed,
                                              const std::string& terminal) {
    // Only handle shift-reduce conflicts
    if (!((existing.type == ActionType::Shift && proposed.type == ActionType::Reduce) ||
         (existing.type == ActionType::Reduce && proposed.type == ActionType::Shift))) {
        return false;
    }

    const Action& shiftAction = (existing.type == ActionType::Shift) ? existing : proposed;
    const Action& reduceAction = (existing.type == ActionType::Reduce) ? existing : proposed;

    // Only apply precedence if both are defined
    bool hasTermPrec = terminalPrecedence.count(terminal);
    bool hasProdPrec = productionPrecedence.count(reduceAction.target);
    
    if (hasTermPrec && hasProdPrec) {
        int termPrec = terminalPrecedence[terminal].first;
        int prodPrec = productionPrecedence[reduceAction.target];
        Associativity assoc = terminalPrecedence[terminal].second;

        if (prodPrec > termPrec) return true;
        if (termPrec > prodPrec) return false;
        
        if (assoc == Associativity::Left) return true;
        if (assoc == Associativity::Right) return false;
        
        throw std::runtime_error("Non-associative operator conflict: " + terminal);
    }
    
    // Default behavior when precedence not fully defined
    if (existing.type == ActionType::Shift) {
        std::cerr << "Warning: Unresolved conflict in state - keeping shift\n";
        return false;
    }
    return true;
}
void LR1ParsingTableGenerator::generateParsingTables() {
    const auto& itemSets = itemSetBuilder.getItemSets();
    const auto& transitions = itemSetBuilder.getTransitions();

    actionTable.resize(itemSets.size());
    gotoTable.resize(itemSets.size());
    expectedTokensPerState.resize(itemSets.size());

    for (size_t state = 0; state < itemSets.size(); ++state) {
        const auto& items = itemSets[state];

        // Process each item in the current state
        for (const auto& item : items) {
            if (item.dotPos < item.rhs.size()) {
                const std::string& symbol = item.rhs[item.dotPos];
                if (grammar.isTerminal(symbol)) {
                    auto it = transitions[state].find(symbol);
                    if (it != transitions[state].end()) {
                        Action newAction{ActionType::Shift, static_cast<int>(it->second)};
                        
                        // Check for existing action
                        auto existingIt = actionTable[state].find(symbol);
                        if (existingIt != actionTable[state].end()) {
                            if (!resolveConflict(existingIt->second, newAction, symbol)) {
                                // Keep existing action if resolution favors it
                                continue;
                            }
                        }
                        
                        actionTable[state][symbol] = newAction;
                        expectedTokensPerState[state].insert(symbol);
                    }
                }
            } else {
                if (item.lhs == grammar.getStartSymbol() + "'") {
                    actionTable[state]["$"] = {ActionType::Accept, -1};
                    expectedTokensPerState[state].insert("$");
                } else {
                    int prodNum = getProductionNumber(item.lhs, item.rhs);
                    const std::string& lookahead = item.lookahead;
                    
                    // Check for existing action
                    auto existingIt = actionTable[state].find(lookahead);
                    if (existingIt != actionTable[state].end()) {
                        Action newAction{ActionType::Reduce, prodNum};
                        
                        if (!resolveConflict(existingIt->second, newAction, lookahead)) {
                            // Keep existing action if resolution favors it
                            continue;
                        }
                    }
                    
                    actionTable[state][lookahead] = {ActionType::Reduce, prodNum};
                    expectedTokensPerState[state].insert(lookahead);
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

void LR1ParsingTableGenerator::printExpectedTokens() const {
    std::cout << "=== Expected Tokens per State ===\n";
    for (size_t state = 0; state < expectedTokensPerState.size(); ++state) {
        std::cout << "State " << state << ":\n  ";
        for (const auto& token : expectedTokensPerState[state]) {
            std::cout << token << " ";
        }
        std::cout << "\n";
    }
}

int LR1ParsingTableGenerator::getProductionNumber(
    const std::string& lhs, const std::vector<std::string>& rhs) const {
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
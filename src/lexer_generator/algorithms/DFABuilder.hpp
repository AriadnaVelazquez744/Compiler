#pragma once

#include <memory>
#include <map>
#include <set>
#include <vector>
#include <string>
#include "../automata/NFA.hpp"
#include "../automata/DFA.hpp"

/**
 * @brief Builder that converts an NFA to a DFA using the subset construction algorithm.
 *
 * Features:
 * - Epsilon-closure computation
 * - Maximal-munch (longest-match) scanning support by prefering the earliest and highest priority accepting rule
 * - Stable tie-breaking by rule order or explicit priority encoded in NFA action_number
 */
class DFABuilder {
public:
    DFABuilder();

    /**
     * @brief Build a DFA from an NFA
     * @param nfa Input NFA
     * @return DFA instance
     */
    std::unique_ptr<DFA> buildDFA(const NFA& nfa);

private:
    // Internal representation of a DFA state's NFA-subset
    struct SubsetInfo {
        std::set<int> nfa_states;       ///< NFA state set for this DFA state
        bool is_accepting = false;      ///< Whether any NFA state is accepting
        int action_number = -1;         ///< Winning action number according to priority/rule order
    };

    // Mapping from NFA-state set signature to DFA state id
    std::map<std::string, int> subset_signature_to_dfa_id_;
    std::vector<SubsetInfo> dfa_subset_infos_;

    // Core steps
    std::set<int> epsilonClosure(const NFA& nfa, const std::set<int>& states) const;
    std::set<int> moveOn(const NFA& nfa, const std::set<int>& states, char c) const;

    // Utilities
    std::string makeSignature(const std::set<int>& states) const;
    SubsetInfo analyzeSubset(const NFA& nfa, const std::set<int>& states) const;
}; 
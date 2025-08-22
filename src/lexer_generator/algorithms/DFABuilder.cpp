#include "DFABuilder.hpp"
#include <queue>
#include <sstream>
#include <limits>

DFABuilder::DFABuilder() {}

std::unique_ptr<DFA> DFABuilder::buildDFA(const NFA& nfa) {
    auto dfa = std::make_unique<DFA>();

    // Start subset = epsilon-closure({NFA start})
    std::set<int> start_set = { nfa.getStartState() };
    std::set<int> start_closure = epsilonClosure(nfa, start_set);

    // Create first DFA state
    std::string start_sig = makeSignature(start_closure);
    int start_dfa_id = dfa->addState();
    dfa->setStartState(start_dfa_id);

    subset_signature_to_dfa_id_.clear();
    dfa_subset_infos_.clear();

    subset_signature_to_dfa_id_[start_sig] = start_dfa_id;
    dfa_subset_infos_.push_back(analyzeSubset(nfa, start_closure));
    if (dfa_subset_infos_.back().is_accepting) {
        dfa->markAccepting(start_dfa_id, dfa_subset_infos_.back().action_number);
    }

    std::queue<std::set<int>> worklist;
    worklist.push(start_closure);

    // Alphabet: derive from NFA transitions by scanning on the fly
    while (!worklist.empty()) {
        std::set<int> current_subset = worklist.front();
        worklist.pop();
        std::string current_sig = makeSignature(current_subset);
        int current_dfa_id = subset_signature_to_dfa_id_[current_sig];

        // Gather outgoing alphabet from this subset
        std::set<char> alphabet;
        for (int nfa_id : current_subset) {
            const NFAState* st = nfa.getState(nfa_id);
            if (!st) continue;
            for (const auto& kv : st->transitions) {
                alphabet.insert(kv.first);
            }
        }

        // For each character, compute next subset
        for (char c : alphabet) {
            std::set<int> move_set = moveOn(nfa, current_subset, c);
            if (move_set.empty()) continue;
            std::set<int> next_closure = epsilonClosure(nfa, move_set);
            if (next_closure.empty()) continue;

            std::string next_sig = makeSignature(next_closure);
            int next_dfa_id;
            auto it = subset_signature_to_dfa_id_.find(next_sig);
            if (it == subset_signature_to_dfa_id_.end()) {
                next_dfa_id = dfa->addState();
                subset_signature_to_dfa_id_[next_sig] = next_dfa_id;
                dfa_subset_infos_.push_back(analyzeSubset(nfa, next_closure));
                if (dfa_subset_infos_.back().is_accepting) {
                    dfa->markAccepting(next_dfa_id, dfa_subset_infos_.back().action_number);
                }
                worklist.push(next_closure);
            } else {
                next_dfa_id = it->second;
            }

            dfa->addTransition(current_dfa_id, c, next_dfa_id);
        }
    }

    return dfa;
}

std::set<int> DFABuilder::epsilonClosure(const NFA& nfa, const std::set<int>& states) const {
    std::set<int> closure = states;
    std::queue<int> q;
    for (int s : states) q.push(s);

    while (!q.empty()) {
        int s = q.front(); q.pop();
        const NFAState* st = nfa.getState(s);
        if (!st) continue;
        for (int t : st->epsilon_transitions) {
            if (!closure.count(t)) {
                closure.insert(t);
                q.push(t);
            }
        }
    }
    return closure;
}

std::set<int> DFABuilder::moveOn(const NFA& nfa, const std::set<int>& states, char c) const {
    std::set<int> out;
    for (int s : states) {
        const NFAState* st = nfa.getState(s);
        if (!st) continue;
        auto it = st->transitions.find(c);
        if (it != st->transitions.end()) {
            out.insert(it->second.begin(), it->second.end());
        }
    }
    return out;
}

std::string DFABuilder::makeSignature(const std::set<int>& states) const {
    std::ostringstream oss;
    bool first = true;
    for (int s : states) {
        if (!first) oss << ',';
        first = false;
        oss << s;
    }
    return oss.str();
}

DFABuilder::SubsetInfo DFABuilder::analyzeSubset(const NFA& nfa, const std::set<int>& states) const {
    SubsetInfo info;
    info.nfa_states = states;

    // Decide accepting and action_number by scanning accepting NFA states.
    // Strategy:
    // - Prefer smallest action_number (assumed to encode higher priority / earlier rule)
    // - If multiple accepting, pick min action_number
    int best_action = std::numeric_limits<int>::max();
    bool any_accept = false;

    for (int s : states) {
        if (nfa.isAccepting(s)) {
            any_accept = true;
            int act = nfa.getActionNumber(s);
            if (act >= 0 && act < best_action) {
                best_action = act;
            }
        }
    }

    info.is_accepting = any_accept;
    info.action_number = any_accept ? best_action : -1;
    return info;
} 
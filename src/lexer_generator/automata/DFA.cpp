#include "DFA.hpp"

DFA::DFA() : start_state_(-1) {}

int DFA::addState() {
    int new_id = static_cast<int>(states_.size());
    states_.emplace_back(new_id);
    if (start_state_ == -1) {
        start_state_ = new_id;
    }
    return new_id;
}

DFAState* DFA::getState(int state_id) {
    if (state_id < 0 || state_id >= static_cast<int>(states_.size())) return nullptr;
    return &states_[state_id];
}

const DFAState* DFA::getState(int state_id) const {
    if (state_id < 0 || state_id >= static_cast<int>(states_.size())) return nullptr;
    return &states_[state_id];
}

void DFA::addTransition(int from_state_id, char c, int to_state_id) {
    DFAState* from = getState(from_state_id);
    if (!from) return;
    from->transitions[c] = to_state_id;
}

void DFA::markAccepting(int state_id, int action_number) {
    DFAState* st = getState(state_id);
    if (!st) return;
    st->is_accepting = true;
    st->action_number = action_number;
} 
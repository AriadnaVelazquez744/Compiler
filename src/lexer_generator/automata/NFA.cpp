#include "NFA.hpp"
#include <stdexcept>

// NFAState implementation
NFAState::NFAState(int state_id) 
    : id(state_id), is_accepting(false), action_number(-1) {}

void NFAState::addTransition(char c, int next_state_id) {
    transitions[c].insert(next_state_id);
}

void NFAState::addEpsilonTransition(int next_state_id) {
    epsilon_transitions.insert(next_state_id);
}

bool NFAState::hasTransition(char c) const {
    return transitions.find(c) != transitions.end();
}

const std::set<int>& NFAState::getTransitions(char c) const {
    static const std::set<int> empty_set;
    auto it = transitions.find(c);
    return (it != transitions.end()) ? it->second : empty_set;
}

const std::set<int>& NFAState::getEpsilonTransitions() const {
    return epsilon_transitions;
}

// NFA implementation
NFA::NFA() : start_state_(-1) {}

int NFA::addState() {
    int state_id = static_cast<int>(states_.size());
    states_.emplace_back(state_id);
    return state_id;
}

NFAState* NFA::getState(int state_id) {
    if (state_id >= 0 && state_id < static_cast<int>(states_.size())) {
        return &states_[state_id];
    }
    return nullptr;
}

const NFAState* NFA::getState(int state_id) const {
    if (state_id >= 0 && state_id < static_cast<int>(states_.size())) {
        return &states_[state_id];
    }
    return nullptr;
}

void NFA::addTransition(int from_state_id, char c, int to_state_id) {
    NFAState* from_state = getState(from_state_id);
    if (!from_state) {
        throw std::out_of_range("Invalid from_state_id: " + std::to_string(from_state_id));
    }
    
    if (!getState(to_state_id)) {
        throw std::out_of_range("Invalid to_state_id: " + std::to_string(to_state_id));
    }
    
    from_state->addTransition(c, to_state_id);
}

void NFA::addEpsilonTransition(int from_state_id, int to_state_id) {
    NFAState* from_state = getState(from_state_id);
    if (!from_state) {
        throw std::out_of_range("Invalid from_state_id: " + std::to_string(from_state_id));
    }
    
    if (!getState(to_state_id)) {
        throw std::out_of_range("Invalid to_state_id: " + std::to_string(to_state_id));
    }
    
    from_state->addEpsilonTransition(to_state_id);
}

void NFA::markAccepting(int state_id, int action_number) {
    NFAState* state = getState(state_id);
    if (!state) {
        throw std::out_of_range("Invalid state_id: " + std::to_string(state_id));
    }
    
    state->is_accepting = true;
    state->action_number = action_number;
    accepting_states_.insert(state_id);
}

bool NFA::isAccepting(int state_id) const {
    const NFAState* state = getState(state_id);
    return state ? state->is_accepting : false;
}

int NFA::getActionNumber(int state_id) const {
    const NFAState* state = getState(state_id);
    return state ? state->action_number : -1;
} 
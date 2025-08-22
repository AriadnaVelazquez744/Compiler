#pragma once

#include <vector>
#include <map>
#include <set>
#include <memory>
#include <string>

/**
 * @brief Represents a state in a Deterministic Finite Automaton (DFA)
 *
 * Each DFA state has at most one outgoing transition per input character.
 * If the state is accepting, it stores the "winning" action number selected
 * during subset construction (based on priority and rule order).
 */
struct DFAState {
    int id;                                                ///< Unique state identifier
    std::map<char, int> transitions;                       ///< Character -> next state ID
    bool is_accepting = false;                             ///< Whether this state accepts input
    int action_number = -1;                                ///< Action number for accepting; -1 if not accepting

    DFAState(int state_id = -1) : id(state_id) {}
};

/**
 * @brief Represents a complete Deterministic Finite Automaton
 */
class DFA {
public:
    DFA();

    /**
     * @brief Add a new state to the DFA
     * @return ID of the newly created state
     */
    int addState();

    /**
     * @brief Get a state by ID
     */
    DFAState* getState(int state_id);

    /**
     * @brief Get a state by ID (const)
     */
    const DFAState* getState(int state_id) const;

    /**
     * @brief Add a transition
     */
    void addTransition(int from_state_id, char c, int to_state_id);

    /**
     * @brief Mark a state as accepting with an action number
     */
    void markAccepting(int state_id, int action_number);

    int getStartState() const { return start_state_; }
    void setStartState(int state_id) { start_state_ = state_id; }

    size_t getStateCount() const { return states_.size(); }
    const std::vector<DFAState>& getStates() const { return states_; }

private:
    std::vector<DFAState> states_;
    int start_state_ = -1;
}; 
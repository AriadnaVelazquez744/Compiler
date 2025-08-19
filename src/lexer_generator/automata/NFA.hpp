#pragma once

#include <vector>
#include <map>
#include <set>
#include <memory>

/**
 * @brief Represents a state in a Non-Deterministic Finite Automaton
 */
struct NFAState {
    int id;                                                           ///< Unique state identifier
    std::map<char, std::set<int>> transitions;                       ///< Character -> set of next state IDs
    std::set<int> epsilon_transitions;                               ///< Epsilon transitions (empty string)
    bool is_accepting;                                                ///< Whether this state accepts input
    int action_number;                                                ///< Action to execute when accepting
    
    NFAState(int state_id = -1);
    
    /**
     * @brief Add a transition on a specific character
     * @param c Character that triggers the transition
     * @param next_state_id ID of the next state
     */
    void addTransition(char c, int next_state_id);
    
    /**
     * @brief Add an epsilon transition
     * @param next_state_id ID of the next state
     */
    void addEpsilonTransition(int next_state_id);
    
    /**
     * @brief Check if there's a transition on a character
     * @param c Character to check
     * @return true if transition exists
     */
    bool hasTransition(char c) const;
    
    /**
     * @brief Get all next states for a character
     * @param c Character to get transitions for
     * @return Set of next state IDs
     */
    const std::set<int>& getTransitions(char c) const;
    
    /**
     * @brief Get all epsilon transitions
     * @return Set of epsilon transition state IDs
     */
    const std::set<int>& getEpsilonTransitions() const;
};

/**
 * @brief Represents a complete Non-Deterministic Finite Automaton
 */
class NFA {
public:
    NFA();
    
    /**
     * @brief Add a new state to the NFA
     * @return ID of the newly created state
     */
    int addState();
    
    /**
     * @brief Get a state by ID
     * @param state_id State identifier
     * @return Pointer to the state, or nullptr if not found
     */
    NFAState* getState(int state_id);
    
    /**
     * @brief Get a state by ID (const version)
     * @param state_id State identifier
     * @return Pointer to the state, or nullptr if not found
     */
    const NFAState* getState(int state_id) const;
    
    /**
     * @brief Add a transition between states
     * @param from_state_id Source state ID
     * @param c Character that triggers the transition
     * @param to_state_id Destination state ID
     */
    void addTransition(int from_state_id, char c, int to_state_id);
    
    /**
     * @brief Add an epsilon transition between states
     * @param from_state_id Source state ID
     * @param to_state_id Destination state ID
     */
    void addEpsilonTransition(int from_state_id, int to_state_id);
    
    /**
     * @brief Mark a state as accepting
     * @param state_id State to mark as accepting
     * @param action_number Action number to execute when accepting
     */
    void markAccepting(int state_id, int action_number);
    
    /**
     * @brief Get the start state ID
     * @return Start state ID
     */
    int getStartState() const { return start_state_; }
    
    /**
     * @brief Set the start state
     * @param state_id ID of the start state
     */
    void setStartState(int state_id) { start_state_ = state_id; }
    
    /**
     * @brief Get all accepting states
     * @return Set of accepting state IDs
     */
    const std::set<int>& getAcceptingStates() const { return accepting_states_; }
    
    /**
     * @brief Get the total number of states
     * @return Number of states in the NFA
     */
    size_t getStateCount() const { return states_.size(); }
    
    /**
     * @brief Get all states
     * @return Vector of all states
     */
    const std::vector<NFAState>& getStates() const { return states_; }
    
    /**
     * @brief Check if a state is accepting
     * @param state_id State ID to check
     * @return true if the state is accepting
     */
    bool isAccepting(int state_id) const;
    
    /**
     * @brief Get the action number for an accepting state
     * @param state_id State ID to get action for
     * @return Action number, or -1 if not accepting
     */
    int getActionNumber(int state_id) const;

private:
    std::vector<NFAState> states_;    ///< All states in the NFA
    int start_state_;                 ///< ID of the start state
    std::set<int> accepting_states_;  ///< Set of accepting state IDs
}; 
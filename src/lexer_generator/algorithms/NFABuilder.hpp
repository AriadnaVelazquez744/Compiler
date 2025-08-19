#pragma once

#include "../regex/RegexNode.hpp"
#include "../automata/NFA.hpp"
#include <memory>

/**
 * @brief Builder for NFAs using Thompson's construction algorithm
 * 
 * This class implements Thompson's construction algorithm to convert
 * regex ASTs into Non-Deterministic Finite Automata.
 */
class NFABuilder {
public:
    NFABuilder();
    
    /**
     * @brief Build an NFA from a regex AST
     * @param root Root node of the regex AST
     * @return NFA representing the regex pattern
     */
    std::unique_ptr<NFA> buildNFA(const std::shared_ptr<RegexNode>& root);
    
    /**
     * @brief Build an NFA from a regex pattern string
     * @param pattern Regex pattern string
     * @return NFA representing the regex pattern
     */
    std::unique_ptr<NFA> buildNFAFromString(const std::string& pattern);
    
    /**
     * @brief Get any errors that occurred during construction
     * @return Vector of error messages
     */
    const std::vector<std::string>& getErrors() const { return errors_; }
    
    /**
     * @brief Check if any errors occurred during construction
     * @return true if there are construction errors
     */
    bool hasErrors() const { return !errors_.empty(); }
    
    /**
     * @brief Clear all construction errors
     */
    void clearErrors() { errors_.clear(); }

private:
    std::vector<std::string> errors_;  ///< Collection of construction error messages
    
    // Thompson construction methods
    std::pair<int, int> buildCharacter(const std::shared_ptr<CharacterNode>& node, NFA& nfa);
    std::pair<int, int> buildCharacterClass(const std::shared_ptr<CharacterClassNode>& node, NFA& nfa);
    std::pair<int, int> buildConcatenation(const std::shared_ptr<ConcatenationNode>& node, NFA& nfa);
    std::pair<int, int> buildAlternation(const std::shared_ptr<AlternationNode>& node, NFA& nfa);
    std::pair<int, int> buildRepetition(const std::shared_ptr<RepetitionNode>& node, NFA& nfa);
    std::pair<int, int> buildGroup(const std::shared_ptr<GroupNode>& node, NFA& nfa);
    std::pair<int, int> buildAnchor(const std::shared_ptr<AnchorNode>& node, NFA& nfa);
    std::pair<int, int> buildEpsilon(const std::shared_ptr<EpsilonNode>& node, NFA& nfa);
    
    // Helper methods
    std::pair<int, int> buildNode(const std::shared_ptr<RegexNode>& node, NFA& nfa);
    void reportError(const std::string& message);
    
    // Thompson construction helpers
    std::pair<int, int> createBasicNFA(char c, NFA& nfa);
    std::pair<int, int> createEpsilonNFA(NFA& nfa);
    std::pair<int, int> createConcatenationNFA(const std::vector<std::pair<int, int>>& sub_nfas, NFA& nfa);
    std::pair<int, int> createAlternationNFA(const std::vector<std::pair<int, int>>& sub_nfas, NFA& nfa);
    std::pair<int, int> createRepetitionNFA(const std::pair<int, int>& sub_nfa, 
                                           RepetitionType type, int min_count, int max_count, NFA& nfa);
}; 
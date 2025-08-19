#include "NFABuilder.hpp"
#include "../regex/RegexParser.hpp"

NFABuilder::NFABuilder() {}

std::unique_ptr<NFA> NFABuilder::buildNFA(const std::shared_ptr<RegexNode>& root) {
    clearErrors();
    
    if (!root) {
        reportError("Cannot build NFA from null root node");
        return nullptr;
    }
    
    auto nfa = std::make_unique<NFA>();
    
    try {
        auto [start, end] = buildNode(root, *nfa);
        nfa->setStartState(start);
        nfa->markAccepting(end, 0); // Default action number
        return nfa;
    } catch (const std::exception& e) {
        reportError(e.what());
        return nullptr;
    }
}

std::unique_ptr<NFA> NFABuilder::buildNFAFromString(const std::string& pattern) {
    clearErrors();
    
    RegexParser parser;
    auto root = parser.parse(pattern);
    
    if (parser.hasErrors()) {
        for (const auto& error : parser.getErrors()) {
            reportError(error);
        }
        return nullptr;
    }
    
    return buildNFA(root);
}

std::pair<int, int> NFABuilder::buildCharacter(const std::shared_ptr<CharacterNode>& node, NFA& nfa) {
    return createBasicNFA(node->getValue(), nfa);
}

std::pair<int, int> NFABuilder::buildCharacterClass(const std::shared_ptr<CharacterClassNode>& node, NFA& nfa) {
    // For now, create a simple NFA that accepts any character in the class
    int start = nfa.addState();
    int end = nfa.addState();
    
    // Add transitions for all characters in the class
    for (const auto& range : node->getRanges()) {
        for (char c = range.first; c <= range.second; ++c) {
            nfa.addTransition(start, c, end);
        }
    }
    
    for (char c : node->getCharacters()) {
        nfa.addTransition(start, c, end);
    }
    
    return {start, end};
}

std::pair<int, int> NFABuilder::buildConcatenation(const std::shared_ptr<ConcatenationNode>& node, NFA& nfa) {
    std::vector<std::pair<int, int>> sub_nfas;
    
    for (const auto& child : node->getChildren()) {
        sub_nfas.push_back(buildNode(child, nfa));
    }
    
    return createConcatenationNFA(sub_nfas, nfa);
}

std::pair<int, int> NFABuilder::buildAlternation(const std::shared_ptr<AlternationNode>& node, NFA& nfa) {
    std::vector<std::pair<int, int>> sub_nfas;
    
    for (const auto& child : node->getChildren()) {
        sub_nfas.push_back(buildNode(child, nfa));
    }
    
    return createAlternationNFA(sub_nfas, nfa);
}

std::pair<int, int> NFABuilder::buildRepetition(const std::shared_ptr<RepetitionNode>& node, NFA& nfa) {
    auto sub_nfa = buildNode(node->getChildren()[0], nfa);
    
    return createRepetitionNFA(sub_nfa, node->getRepetitionType(), 
                              node->getMinCount(), node->getMaxCount(), nfa);
}

std::pair<int, int> NFABuilder::buildGroup(const std::shared_ptr<GroupNode>& node, NFA& nfa) {
    if (node->getChildren().empty()) {
        return createEpsilonNFA(nfa);
    }
    
    return buildNode(node->getChildren()[0], nfa);
}

std::pair<int, int> NFABuilder::buildAnchor(const std::shared_ptr<AnchorNode>& node, NFA& nfa) {
    // For now, create a simple accepting NFA for anchors
    int start = nfa.addState();
    int end = nfa.addState();
    
    // Add epsilon transition (anchors don't consume characters)
    nfa.addEpsilonTransition(start, end);
    
    return {start, end};
}

std::pair<int, int> NFABuilder::buildEpsilon(const std::shared_ptr<EpsilonNode>& node, NFA& nfa) {
    return createEpsilonNFA(nfa);
}

std::pair<int, int> NFABuilder::buildNode(const std::shared_ptr<RegexNode>& node, NFA& nfa) {
    switch (node->getType()) {
        case RegexNodeType::CHARACTER:
            return buildCharacter(std::static_pointer_cast<CharacterNode>(node), nfa);
        case RegexNodeType::CHARACTER_CLASS:
            return buildCharacterClass(std::static_pointer_cast<CharacterClassNode>(node), nfa);
        case RegexNodeType::CONCATENATION:
            return buildConcatenation(std::static_pointer_cast<ConcatenationNode>(node), nfa);
        case RegexNodeType::ALTERNATION:
            return buildAlternation(std::static_pointer_cast<AlternationNode>(node), nfa);
        case RegexNodeType::REPETITION:
            return buildRepetition(std::static_pointer_cast<RepetitionNode>(node), nfa);
        case RegexNodeType::GROUP:
            return buildGroup(std::static_pointer_cast<GroupNode>(node), nfa);
        case RegexNodeType::ANCHOR:
            return buildAnchor(std::static_pointer_cast<AnchorNode>(node), nfa);
        case RegexNodeType::EPSILON:
            return buildEpsilon(std::static_pointer_cast<EpsilonNode>(node), nfa);
        default:
            reportError("Unknown regex node type");
            return {-1, -1};
    }
}

void NFABuilder::reportError(const std::string& message) {
    errors_.push_back(message);
}

// Thompson construction helpers
std::pair<int, int> NFABuilder::createBasicNFA(char c, NFA& nfa) {
    int start = nfa.addState();
    int end = nfa.addState();
    
    nfa.addTransition(start, c, end);
    
    return {start, end};
}

std::pair<int, int> NFABuilder::createEpsilonNFA(NFA& nfa) {
    int start = nfa.addState();
    int end = nfa.addState();
    
    nfa.addEpsilonTransition(start, end);
    
    return {start, end};
}

std::pair<int, int> NFABuilder::createConcatenationNFA(const std::vector<std::pair<int, int>>& sub_nfas, NFA& nfa) {
    if (sub_nfas.empty()) {
        return createEpsilonNFA(nfa);
    }
    
    if (sub_nfas.size() == 1) {
        return sub_nfas[0];
    }
    
    // Connect sub-NFAs in sequence
    for (size_t i = 0; i < sub_nfas.size() - 1; ++i) {
        nfa.addEpsilonTransition(sub_nfas[i].second, sub_nfas[i + 1].first);
    }
    
    return {sub_nfas[0].first, sub_nfas.back().second};
}

std::pair<int, int> NFABuilder::createAlternationNFA(const std::vector<std::pair<int, int>>& sub_nfas, NFA& nfa) {
    if (sub_nfas.empty()) {
        return createEpsilonNFA(nfa);
    }
    
    if (sub_nfas.size() == 1) {
        return sub_nfas[0];
    }
    
    int start = nfa.addState();
    int end = nfa.addState();
    
    // Connect start to all sub-NFAs
    for (const auto& sub_nfa : sub_nfas) {
        nfa.addEpsilonTransition(start, sub_nfa.first);
    }
    
    // Connect all sub-NFAs to end
    for (const auto& sub_nfa : sub_nfas) {
        nfa.addEpsilonTransition(sub_nfa.second, end);
    }
    
    return {start, end};
}

std::pair<int, int> NFABuilder::createRepetitionNFA(const std::pair<int, int>& sub_nfa, 
                                                   RepetitionType type, int min_count, int max_count, NFA& nfa) {
    int start = nfa.addState();
    int end = nfa.addState();
    
    switch (type) {
        case RepetitionType::ZERO_OR_MORE:
            // a* = ε | a | aa | aaa | ...
            nfa.addEpsilonTransition(start, end);  // 0 repetitions
            nfa.addEpsilonTransition(start, sub_nfa.first);  // 1+ repetitions
            nfa.addEpsilonTransition(sub_nfa.second, sub_nfa.first);  // Loop back
            nfa.addEpsilonTransition(sub_nfa.second, end);  // Exit loop
            break;
            
        case RepetitionType::ONE_OR_MORE:
            // a+ = a | aa | aaa | ...
            nfa.addEpsilonTransition(start, sub_nfa.first);  // 1+ repetitions
            nfa.addEpsilonTransition(sub_nfa.second, sub_nfa.first);  // Loop back
            nfa.addEpsilonTransition(sub_nfa.second, end);  // Exit loop
            break;
            
        case RepetitionType::ZERO_OR_ONE:
            // a? = ε | a
            nfa.addEpsilonTransition(start, end);  // 0 repetitions
            nfa.addEpsilonTransition(start, sub_nfa.first);  // 1 repetition
            nfa.addEpsilonTransition(sub_nfa.second, end);
            break;
            
        default:
            // For now, handle as simple repetition
            nfa.addEpsilonTransition(start, sub_nfa.first);
            nfa.addEpsilonTransition(sub_nfa.second, end);
            break;
    }
    
    return {start, end};
} 
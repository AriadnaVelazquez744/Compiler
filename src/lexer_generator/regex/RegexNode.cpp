#include "RegexNode.hpp"
#include "RegexVisitor.hpp"

// Base RegexNode implementation
RegexNode::RegexNode(RegexNodeType type) : type_(type) {}

void RegexNode::addChild(std::shared_ptr<RegexNode> child) {
    if (child) {
        children_.push_back(child);
    }
}

void RegexNode::setChildren(std::vector<std::shared_ptr<RegexNode>> children) {
    children_ = std::move(children);
}

// CharacterNode implementation
CharacterNode::CharacterNode(char value) : RegexNode(RegexNodeType::CHARACTER), value_(value) {}

void CharacterNode::accept(RegexVisitor& visitor) {
    visitor.visitCharacter(*this);
}

// CharacterClassNode implementation
CharacterClassNode::CharacterClassNode(bool negated) 
    : RegexNode(RegexNodeType::CHARACTER_CLASS), negated_(negated) {}

void CharacterClassNode::addRange(char start, char end) {
    if (start <= end) {
        ranges_.emplace_back(start, end);
    }
}

void CharacterClassNode::addCharacter(char c) {
    characters_.push_back(c);
}

void CharacterClassNode::accept(RegexVisitor& visitor) {
    visitor.visitCharacterClass(*this);
}

// ConcatenationNode implementation
ConcatenationNode::ConcatenationNode() : RegexNode(RegexNodeType::CONCATENATION) {}

void ConcatenationNode::accept(RegexVisitor& visitor) {
    visitor.visitConcatenation(*this);
}

// AlternationNode implementation
AlternationNode::AlternationNode() : RegexNode(RegexNodeType::ALTERNATION) {}

void AlternationNode::accept(RegexVisitor& visitor) {
    visitor.visitAlternation(*this);
}

// RepetitionNode implementation
RepetitionNode::RepetitionNode(RepetitionType type, int min_count, int max_count)
    : RegexNode(RegexNodeType::REPETITION), type_(type), min_count_(min_count), max_count_(max_count) {}

void RepetitionNode::accept(RegexVisitor& visitor) {
    visitor.visitRepetition(*this);
}

// GroupNode implementation
GroupNode::GroupNode() : RegexNode(RegexNodeType::GROUP) {}

void GroupNode::accept(RegexVisitor& visitor) {
    visitor.visitGroup(*this);
}

// AnchorNode implementation
AnchorNode::AnchorNode(AnchorType type) : RegexNode(RegexNodeType::ANCHOR), type_(type) {}

void AnchorNode::accept(RegexVisitor& visitor) {
    visitor.visitAnchor(*this);
}

// EpsilonNode implementation
EpsilonNode::EpsilonNode() : RegexNode(RegexNodeType::EPSILON) {}

void EpsilonNode::accept(RegexVisitor& visitor) {
    visitor.visitEpsilon(*this);
} 
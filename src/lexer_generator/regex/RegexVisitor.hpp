#pragma once

// Forward declarations
class CharacterNode;
class CharacterClassNode;
class ConcatenationNode;
class AlternationNode;
class RepetitionNode;
class GroupNode;
class AnchorNode;
class EpsilonNode;

/**
 * @brief Abstract visitor interface for traversing regex AST
 * 
 * This visitor pattern allows different operations to be performed
 * on the regex AST without modifying the node classes.
 */
class RegexVisitor {
public:
    virtual ~RegexVisitor() = default;
    
    // Visit methods for each node type
    virtual void visitCharacter(const CharacterNode& node) = 0;
    virtual void visitCharacterClass(const CharacterClassNode& node) = 0;
    virtual void visitConcatenation(const ConcatenationNode& node) = 0;
    virtual void visitAlternation(const AlternationNode& node) = 0;
    virtual void visitRepetition(const RepetitionNode& node) = 0;
    virtual void visitGroup(const GroupNode& node) = 0;
    virtual void visitAnchor(const AnchorNode& node) = 0;
    virtual void visitEpsilon(const EpsilonNode& node) = 0;
}; 
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <variant>

/**
 * @brief Types of regex nodes in the AST
 */
enum class RegexNodeType {
    CHARACTER,          ///< Single character (e.g., 'a', 'b')
    CHARACTER_CLASS,    ///< Character class (e.g., [a-z], [^0-9])
    CONCATENATION,      ///< Sequence of patterns (e.g., ab, abc)
    ALTERNATION,        ///< Choice between patterns (e.g., a|b, x|y|z)
    REPETITION,         ///< Repetition patterns (e.g., a*, a+, a?, a{n,m})
    GROUP,              ///< Grouping with parentheses (e.g., (a|b)*)
    ANCHOR,             ///< Anchors (e.g., ^, $, \b, \B)
    EPSILON             ///< Empty string (epsilon transition)
};

/**
 * @brief Repetition types for regex patterns
 */
enum class RepetitionType {
    ZERO_OR_MORE,       ///< * (Kleene star)
    ONE_OR_MORE,        ///< + (Kleene plus)
    ZERO_OR_ONE,        ///< ? (optional)
    EXACT,              ///< {n} (exact count)
    RANGE               ///< {n,m} (range count)
};

/**
 * @brief Base class for all regex AST nodes
 */
class RegexNode {
public:
    explicit RegexNode(RegexNodeType type);
    virtual ~RegexNode() = default;
    
    // Getters
    RegexNodeType getType() const { return type_; }
    const std::vector<std::shared_ptr<RegexNode>>& getChildren() const { return children_; }
    
    // Child management
    void addChild(std::shared_ptr<RegexNode> child);
    void setChildren(std::vector<std::shared_ptr<RegexNode>> children);
    
    // Utility methods
    bool isLeaf() const { return children_.empty(); }
    size_t getChildCount() const { return children_.size(); }
    
    // Visitor pattern support
    virtual void accept(class RegexVisitor& visitor) = 0;

protected:
    RegexNodeType type_;
    std::vector<std::shared_ptr<RegexNode>> children_;
};

/**
 * @brief Single character node
 */
class CharacterNode : public RegexNode {
public:
    explicit CharacterNode(char value);
    
    char getValue() const { return value_; }
    void accept(class RegexVisitor& visitor) override;

private:
    char value_;
};

/**
 * @brief Character class node (e.g., [a-z], [^0-9])
 */
class CharacterClassNode : public RegexNode {
public:
    CharacterClassNode(bool negated = false);
    
    void addRange(char start, char end);
    void addCharacter(char c);
    bool isNegated() const { return negated_; }
    const std::vector<std::pair<char, char>>& getRanges() const { return ranges_; }
    const std::vector<char>& getCharacters() const { return characters_; }
    
    void accept(class RegexVisitor& visitor) override;

private:
    bool negated_;
    std::vector<std::pair<char, char>> ranges_;      ///< Character ranges (e.g., a-z)
    std::vector<char> characters_;                   ///< Individual characters
};

/**
 * @brief Concatenation node (sequence of patterns)
 */
class ConcatenationNode : public RegexNode {
public:
    ConcatenationNode();
    
    void accept(class RegexVisitor& visitor) override;
};

/**
 * @brief Alternation node (choice between patterns)
 */
class AlternationNode : public RegexNode {
public:
    AlternationNode();
    
    void accept(class RegexVisitor& visitor) override;
};

/**
 * @brief Repetition node (e.g., a*, a+, a?, a{n,m})
 */
class RepetitionNode : public RegexNode {
public:
    RepetitionNode(RepetitionType type, int min_count = 0, int max_count = -1);
    
    RepetitionType getRepetitionType() const { return type_; }
    int getMinCount() const { return min_count_; }
    int getMaxCount() const { return max_count_; }
    
    void accept(class RegexVisitor& visitor) override;

private:
    RepetitionType type_;
    int min_count_;
    int max_count_;
};

/**
 * @brief Grouping node (parentheses)
 */
class GroupNode : public RegexNode {
public:
    GroupNode();
    
    void accept(class RegexVisitor& visitor) override;
};

/**
 * @brief Anchor node (^, $, \b, \B)
 */
class AnchorNode : public RegexNode {
public:
    enum class AnchorType {
        START_OF_LINE,      ///< ^
        END_OF_LINE,        ///< $
        WORD_BOUNDARY,      ///< \b
        NON_WORD_BOUNDARY   ///< \B
    };
    
    explicit AnchorNode(AnchorType type);
    
    AnchorType getAnchorType() const { return type_; }
    void accept(class RegexVisitor& visitor) override;

private:
    AnchorType type_;
};

/**
 * @brief Epsilon node (empty string)
 */
class EpsilonNode : public RegexNode {
public:
    EpsilonNode();
    
    void accept(class RegexVisitor& visitor) override;
}; 
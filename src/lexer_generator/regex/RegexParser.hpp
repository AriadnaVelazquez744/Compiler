#pragma once

#include "RegexNode.hpp"
#include <string>
#include <memory>
#include <vector>

/**
 * @brief Parser for regex patterns that builds AST
 * 
 * This parser converts regex strings into Abstract Syntax Trees
 * that can be used to build finite automata.
 */
class RegexParser {
public:
    /**
     * @brief Parse a regex pattern string into an AST
     * @param pattern The regex pattern to parse
     * @return Root node of the parsed AST
     * @throws std::runtime_error if parsing fails
     */
    std::shared_ptr<RegexNode> parse(const std::string& pattern);
    
    /**
     * @brief Get parsing errors encountered during parsing
     * @return Vector of error messages
     */
    const std::vector<std::string>& getErrors() const { return errors_; }
    
    /**
     * @brief Check if any parsing errors occurred
     * @return true if there are parsing errors
     */
    bool hasErrors() const { return !errors_.empty(); }
    
    /**
     * @brief Clear all parsing errors
     */
    void clearErrors() { errors_.clear(); }

private:
    std::vector<std::string> errors_;  ///< Collection of parsing error messages
    std::string input_;                ///< Input pattern being parsed
    size_t pos_;                       ///< Current position in input
    size_t length_;                    ///< Length of input
    
    // Parsing methods
    std::shared_ptr<RegexNode> parseExpression();
    std::shared_ptr<RegexNode> parseTerm();
    std::shared_ptr<RegexNode> parseFactor();
    std::shared_ptr<RegexNode> parseAtom();
    std::shared_ptr<RegexNode> parseCharacterClass();
    std::shared_ptr<RegexNode> parseRepetition(std::shared_ptr<RegexNode> atom);
    std::shared_ptr<RegexNode> parseGroup();
    
    // Helper methods
    char peek() const;
    char get();
    void advance();
    bool isAtEnd() const;
    bool match(char expected);
    void skipWhitespace();
    
    // Error reporting
    void reportError(const std::string& message);
    
    // Character classification
    bool isSpecialChar(char c) const;
    bool isQuantifier(char c) const;
    char unescapeChar(char c) const;
}; 
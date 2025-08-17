#pragma once

#include "../core/TokenPattern.hpp"
#include <vector>
#include <string>
#include <memory>

/**
 * @brief Parser for .token files that define token patterns
 * 
 * This parser reads .token files with the following format:
 * TokenType : RegexPattern [-> DataType] [| Priority] [| Action]
 * 
 * Examples:
 * NUMBER : [0-9]+(\.[0-9]+)? -> int
 * STRING : \"[^\"]*\" -> string
 * ID : [a-zA-Z_][a-zA-Z0-9_]*
 * ADD : \+ | 10
 * 
 * Lines starting with # are treated as comments and ignored.
 */
class TokenFileParser {
public:
    /**
     * @brief Parse a .token file and return vector of TokenPattern objects
     * @param filename Path to the .token file
     * @return Vector of parsed TokenPattern objects
     * @throws std::runtime_error if file cannot be read or parsed
     */
    std::vector<std::shared_ptr<TokenPattern>> parseTokenFile(const std::string& filename);
    
    /**
     * @brief Parse a single line from a .token file
     * @param line The line to parse
     * @param line_number Line number for error reporting
     * @return Parsed TokenPattern object or nullptr if line is comment/empty
     * @throws std::runtime_error if line format is invalid
     */
    std::shared_ptr<TokenPattern> parsePatternLine(const std::string& line, int line_number);
    
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
    
    /**
     * @brief Strip comments from a line
     * @param line Line to strip comments from
     * @return Line with comments removed
     */
    std::string stripComments(const std::string& line) const;
    
    /**
     * @brief Trim whitespace from beginning and end of string
     * @param str String to trim
     * @return Trimmed string
     */
    std::string trim(const std::string& str) const;
    
    /**
     * @brief Split a string by delimiter
     * @param str String to split
     * @param delimiter Delimiter character
     * @return Vector of substrings
     */
    std::vector<std::string> split(const std::string& str, char delimiter) const;
    
    /**
     * @brief Validate regex pattern syntax (basic validation)
     * @param pattern Regex pattern to validate
     * @return true if pattern appears valid
     */
    bool isValidRegex(const std::string& pattern) const;
    
    /**
     * @brief Report a parsing error
     * @param message Error message
     * @param line_number Line number where error occurred
     */
    void reportError(const std::string& message, int line_number);
    
    /**
     * @brief Parse priority value from string
     * @param priority_str String representation of priority
     * @return Parsed priority value
     */
    int parsePriority(const std::string& priority_str) const;
    
    /**
     * @brief Parse data type from string
     * @param type_str String representation of data type
     * @return Parsed data type string
     */
    std::string parseDataType(const std::string& type_str) const;
}; 
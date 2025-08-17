#pragma once

#include "TokenPattern.hpp"
#include "GeneratorConfig.hpp"
#include "../parser/TokenFileParser.hpp"
#include <vector>
#include <memory>
#include <string>

/**
 * @brief Main class that orchestrates the lexer generation process
 * 
 * This class coordinates all the components needed to generate a lexer:
 * 1. Parse .token files
 * 2. Generate TokenType enums
 * 3. Generate tokenTypeToString functions
 * 4. Generate the lexer class itself
 * 5. Integrate with the build system
 */
class LexerGenerator {
public:
    /**
     * @brief Constructor with configuration
     * @param config Configuration options for the generator
     */
    explicit LexerGenerator(const GeneratorConfig& config);
    
    /**
     * @brief Destructor
     */
    ~LexerGenerator() = default;
    
    /**
     * @brief Add a token pattern manually
     * @param pattern TokenPattern object to add
     */
    void addPattern(const std::shared_ptr<TokenPattern>& pattern);
    
    /**
     * @brief Load token patterns from a .token file
     * @param filename Path to the .token file
     * @return true if loaded successfully
     */
    bool loadTokenFile(const std::string& filename);
    
    /**
     * @brief Generate all necessary files for the lexer
     * @return true if generation was successful
     */
    bool generateLexer();
    
    /**
     * @brief Generate only the TokenType enum and related code
     * @return true if generation was successful
     */
    bool generateTokenTypes();
    
    /**
     * @brief Generate only the lexer class
     * @return true if generation was successful
     */
    bool generateLexerClass();
    
    /**
     * @brief Update the main Makefile to use generated files
     * @return true if update was successful
     */
    bool updateMakefile();
    
    /**
     * @brief Get the current configuration
     * @return Reference to the current configuration
     */
    const GeneratorConfig& getConfig() const { return config_; }
    
    /**
     * @brief Get all loaded token patterns
     * @return Vector of token patterns
     */
    const std::vector<std::shared_ptr<TokenPattern>>& getPatterns() const { return patterns_; }
    
    /**
     * @brief Check if any errors occurred during generation
     * @return true if there are errors
     */
    bool hasErrors() const { return !errors_.empty(); }
    
    /**
     * @brief Get all error messages
     * @return Vector of error messages
     */
    const std::vector<std::string>& getErrors() const { return errors_; }
    
    /**
     * @brief Clear all error messages
     */
    void clearErrors() { errors_.clear(); }

private:
    GeneratorConfig config_;                                    ///< Configuration options
    std::vector<std::shared_ptr<TokenPattern>> patterns_;      ///< Token patterns to generate
    TokenFileParser parser_;                                    ///< Parser for .token files
    std::vector<std::string> errors_;                          ///< Error messages
    
    /**
     * @brief Report an error during generation
     * @param message Error message
     */
    void reportError(const std::string& message);
    
    /**
     * @brief Validate all token patterns
     * @return true if all patterns are valid
     */
    bool validatePatterns() const;
    
    /**
     * @brief Sort patterns by priority for proper disambiguation
     */
    void sortPatternsByPriority();
    
    /**
     * @brief Generate the TokenType enum header
     * @return Generated header content
     */
    std::string generateTokenTypeHeader() const;
    
    /**
     * @brief Generate the tokenTypeToString function
     * @return Generated function content
     */
    std::string generateTokenTypeToString() const;
    
    /**
     * @brief Generate the lexer class header
     * @return Generated header content
     */
    std::string generateLexerHeader() const;
    
    /**
     * @brief Generate the lexer class implementation
     * @return Generated implementation content
     */
    std::string generateLexerImplementation() const;
    
    /**
     * @brief Write content to a file
     * @param filename File to write to
     * @param content Content to write
     * @return true if write was successful
     */
    bool writeFile(const std::string& filename, const std::string& content) const;
    
    /**
     * @brief Backup existing file if it exists
     * @param filename File to backup
     * @return true if backup was successful
     */
    bool backupFile(const std::string& filename) const;
}; 
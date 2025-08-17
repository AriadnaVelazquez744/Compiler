#pragma once

#include <string>

/**
 * @brief Configuration options for the lexer generator
 * 
 * This class holds all the configuration options that control
 * how the lexer generator behaves and what it produces.
 */
class GeneratorConfig {
public:
    /**
     * @brief Default constructor with sensible defaults
     */
    GeneratorConfig();

    // Output configuration
    std::string output_directory = ".build/generated";  ///< Directory for generated files
    std::string class_name = "GeneratedLexer";          ///< Name of the generated lexer class
    std::string namespace_name = "";                    ///< C++ namespace for generated code
    std::string header_file = "GeneratedLexer.hpp";     ///< Generated header file name
    std::string source_file = "GeneratedLexer.cpp";     ///< Generated source file name
    std::string token_types_file = "GeneratedTokenTypes.hpp"; ///< Generated token types file
    
    // Code generation options
    bool generate_tests = false;                        ///< Generate unit tests
    bool optimize_dfa = true;                           ///< Optimize DFA for performance
    bool compress_tables = true;                        ///< Compress transition tables
    std::string language = "C++";                       ///< Target language (C++, C, etc.)
    
    // Token handling options
    bool generate_token_values = true;                  ///< Generate token value storage
    bool generate_location_tracking = true;             ///< Generate source location tracking
    bool generate_error_handling = true;                ///< Generate error handling code
    
    // Build integration options
    bool update_makefile = true;                        ///< Update main Makefile
    bool generate_build_scripts = false;                ///< Generate additional build scripts
    bool backup_existing_files = true;                  ///< Backup files before overwriting
    
    // Validation options
    bool strict_mode = false;                           ///< Strict validation of patterns
    bool warnings_as_errors = false;                    ///< Treat warnings as errors
    bool verbose = false;                               ///< Enable verbose output
    
    /**
     * @brief Load configuration from a file
     * @param filename Configuration file path
     * @return true if loaded successfully
     */
    bool loadFromFile(const std::string& filename);
    
    /**
     * @brief Save configuration to a file
     * @param filename Configuration file path
     * @return true if saved successfully
     */
    bool saveToFile(const std::string& filename) const;
    
    /**
     * @brief Validate configuration settings
     * @return true if configuration is valid
     */
    bool isValid() const;
    
    /**
     * @brief Get full path for generated header file
     * @return Full path including directory
     */
    std::string getHeaderFilePath() const;
    
    /**
     * @brief Get full path for generated source file
     * @return Full path including directory
     */
    std::string getSourceFilePath() const;
    
    /**
     * @brief Get full path for generated token types file
     * @return Full path including directory
     */
    std::string getTokenTypesFilePath() const;
    
    /**
     * @brief Create output directory if it doesn't exist
     * @return true if directory created or already exists
     */
    bool ensureOutputDirectory() const;
}; 
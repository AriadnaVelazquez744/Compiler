#include <iostream>
#include <string>
#include <memory>
#include "../core/LexerGenerator.hpp"
#include "../core/GeneratorConfig.hpp"

int main() {
    try {
        std::cout << "=== Lexer Generator for Hulk Language ===\n";
        
        // Configure the generator
        GeneratorConfig config;
        config.output_directory = "src/lexer/.build";
        config.setClassName("Lexer"); // This will also update file names
        config.namespace_name = "hulk";
        config.verbose = true;
        config.update_makefile = false; // We don't want to update the main Makefile
        
        std::cout << "Output directory: " << config.output_directory << "\n";
        std::cout << "Class name: " << config.class_name << "\n";
        std::cout << "Namespace: " << config.namespace_name << "\n";
        std::cout << "========================\n\n";
        
        // Create lexer generator
        LexerGenerator generator(config);
        
        // Load token file
        std::cout << "Loading token file: src/lexer/hulk_language.tokens\n";
        if (!generator.loadTokenFile("src/lexer/hulk_language.tokens")) {
            std::cerr << "Failed to load token file\n";
            if (generator.hasErrors()) {
                for (const auto& error : generator.getErrors()) {
                    std::cerr << "  " << error << "\n";
                }
            }
            return 1;
        }
        
        std::cout << "Loaded " << generator.getPatterns().size() << " token patterns\n\n";
        
        // Display loaded patterns
        std::cout << "Token Patterns:\n";
        for (const auto& pattern : generator.getPatterns()) {
            std::cout << "  " << pattern->getName() << " : " << pattern->getRegexPattern();
            if (pattern->hasDataType()) {
                std::cout << " -> " << pattern->getDataType().value();
            }
            if (pattern->getPriority() > 0) {
                std::cout << " | Priority: " << pattern->getPriority();
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        
        // Generate lexer
        std::cout << "Generating lexer...\n";
        if (!generator.generateLexer()) {
            std::cerr << "Failed to generate lexer\n";
            if (generator.hasErrors()) {
                for (const auto& error : generator.getErrors()) {
                    std::cerr << "  " << error << "\n";
                }
            }
            return 1;
        }
        
        std::cout << "\n✅ Lexer generation completed successfully!\n";
        std::cout << "Generated files in " << config.output_directory << ":\n";
        std::cout << "  - " << config.getTokenTypesFilePath() << "\n";
        std::cout << "  - " << config.getHeaderFilePath() << "\n";
        std::cout << "  - " << config.getSourceFilePath() << "\n";
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
} 
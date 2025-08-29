#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <filesystem> // C++17

// Include the generated lexer
#include "lexer/.build/Lexer.hpp"
#include "lexer/.build/TokenTypes.hpp"

std::string readFile(const std::string& filename) {
    // Ensure file has .hulk extension
    if (!std::filesystem::path(filename).has_extension() ||
        std::filesystem::path(filename).extension() != ".hulk") {
        throw std::invalid_argument("Archivo debe tener extensión '.hulk'.");
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char** argv) {
    const char* filename = (argc >= 2) ? argv[1] : "script.hulk";

    std::string source;
    try {
        source = readFile(filename);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // 1. LEXER: Convert source code into tokens using the generated Lexer
    std::cout << "=== Hulk Compiler - Lexer Phase ===\n";
    std::cout << "Processing file: " << filename << "\n";
    std::cout << "Source length: " << source.length() << " characters\n\n";

    try {
        // Create the generated lexer
        Lexer lexer(source);
        
        // Tokenize the entire input
        std::vector<Token> tokens = lexer.tokenize();
        
        // Display all tokens found
        std::cout << "Tokens encontrados (" << tokens.size() << "):\n";
        for (size_t i = 0; i < tokens.size(); ++i) {
            const auto& token = tokens[i];
            std::cout << "  [" << i << "] " << tokenTypeToString(token.type) 
                      << " => '" << token.lexeme << "'"
                      << " at line " << token.location.line 
                      << ", column " << token.location.column << "\n";
        }
        
        std::cout << "\n✅ Lexer phase completed successfully!\n";
        std::cout << "Total tokens: " << tokens.size() << "\n";
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error en fase léxica: " << e.what() << "\n";
        return 1;
    }
}

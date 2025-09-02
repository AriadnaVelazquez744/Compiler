#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <filesystem> // C++17

// Include the generated lexer
#include "lexer/.build/Lexer.hpp"
#include "lexer/.build/TokenTypes.hpp"

#include "parser/ParserDriver.hpp"
#include "parser/grammar/SemanticActionDispatcher.hpp"
#include "ast/AST.hpp"
#include "ast/ASTPrinter.hpp"

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

void printTokens(const std::vector<std::shared_ptr<Token>>& tokens) {
    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto& token = tokens[i];
        std::cout << "  [" << i << "] " << tokenTypeToString(token->type) 
                  << " => '" << token->lexeme << "'"
                  << " at line " << token->location.line 
                  << ", column " << token->location.column << "\n";
    }
}


bool is_valid_ast(const std::vector<std::shared_ptr<ASTNode>>& nodes) {
    if (nodes.empty()) {
        std::cerr << "AST vacío: ningún nodo generado" << std::endl;
        return false;
    }
    for (const auto& node : nodes) {
        if (!node) {
            std::cerr << "AST contiene nodos nulos" << std::endl;
            return false;
        }
    }
    return true;
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

    std::vector<std::shared_ptr<Token>> tokens;
    std::vector<std::shared_ptr<Token>> error_tokens;
    bool lexer_success = false;
    
    try {
        // Create the generated lexer
        Lexer lexer(source);
        
        // Tokenize the entire input
        tokens = lexer.tokenize();
        
        // Check for error tokens (those with 'ERROR' in their type)
        for (const auto& token : tokens) {
            std::string token_type = tokenTypeToString(token->type);
            if (token_type.find("ERROR") != std::string::npos) {
                error_tokens.push_back(token); // Convert shared_ptr back to Token
            }
        }
        
        // Determine if lexer phase succeeded
        lexer_success = error_tokens.empty();
        
    } catch (const std::exception& e) {
        std::cerr << "Error en fase léxica: " << e.what() << "\n";
        return 1;
    }
    
    // Process results outside of try-catch for proper resource cleanup
    if (!lexer_success) {
        // Lexer phase failed - print error tokens
        std::cout << "❌ Lexer phase failed!\n";
        std::cout << "Tokens con errores encontrados (" << error_tokens.size() << "):\n";
        printTokens(error_tokens);
        return 1;
    } else {
        // Lexer phase succeeded - print all tokens
        std::cout << "Tokens encontrados (" << tokens.size() << "):\n";
        printTokens(tokens);
        
        std::cout << "\n✅ Lexer phase completed successfully!\n";
        std::cout << "Total tokens: " << tokens.size() << "\n";
    }

    // 2. PARSER: Use generated tables directly
    SemanticActionDispatcher dispatcher;
    ParserDriver driver(dispatcher);

    ParseResult result = driver.parse(tokens);

    if (!result.errors.empty()) {
        std::cerr << "Errores de análisis sintáctico:\n";
        for (const std::string& e : result.errors)
            std::cerr << "  " << e << "\n";
        return 1;
    }

    if (!is_valid_ast(result.ast)) {
        std::cerr << "Error: AST inválido.\n";
        return 1;
    }

    std::cout << "\n=== AST Structure ===\n";
    ASTPrinter printer;
    for (const auto& node : result.ast) {
        std::cout << "\nRoot Node:\n";
        node->accept(printer);
    }
    std::cout << "\n=== End AST Structure ===\n";

    return 0;
}

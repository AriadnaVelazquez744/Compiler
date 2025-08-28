#include "LexerGenerator.hpp"
#include "../codegen/CodeGenerator.hpp"
#include "../regex/RegexParser.hpp"
#include "../algorithms/NFABuilder.hpp"
#include "../algorithms/DFABuilder.hpp"
#include "../automata/DFA.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <set>

LexerGenerator::LexerGenerator(const GeneratorConfig& config)
    : config_(config) {
    // Ensure output directory exists
    config_.ensureOutputDirectory();
}

void LexerGenerator::addPattern(const std::shared_ptr<TokenPattern>& pattern) {
    if (pattern) {
        patterns_.push_back(pattern);
    }
}

bool LexerGenerator::loadTokenFile(const std::string& filename) {
    try {
        auto new_patterns = parser_.parseTokenFile(filename);
        patterns_.insert(patterns_.end(), new_patterns.begin(), new_patterns.end());
        
        // Sort patterns by priority after loading
        sortPatternsByPriority();
        
        return true;
    } catch (const std::exception& e) {
        reportError("Failed to load token file '" + filename + "': " + e.what());
        return false;
    }
}

bool LexerGenerator::generateLexer() {
    clearErrors();
    
    // Validate configuration
    if (!config_.isValid()) {
        reportError("Invalid configuration");
        return false;
    }
    
    // Validate patterns
    if (!validatePatterns()) {
        return false;
    }
    
    // Generate token types first
    if (!generateTokenTypes()) {
        return false;
    }
    
    // Generate lexer class
    if (!generateLexerClass()) {
        return false;
    }
    
    // Update Makefile if requested
    if (config_.update_makefile) {
        if (!updateMakefile()) {
            reportError("Failed to update Makefile");
            return false;
        }
    }
    
    return !hasErrors();
}

bool LexerGenerator::generateTokenTypes() {
    try {
        // Build NFAs and DFAs for all patterns
        std::cout << "Building NFAs and DFAs for " << patterns_.size() << " token patterns..." << std::endl;
        
        RegexParser regex_parser;
        NFABuilder nfa_builder;
        DFABuilder dfa_builder;
        
        std::vector<std::unique_ptr<DFA>> dfas;
        dfas.reserve(patterns_.size());
        
        for (const auto& pattern : patterns_) {
            try {
                auto ast = regex_parser.parse(pattern->getRegexPattern());
                auto nfa = nfa_builder.buildNFA(ast);
                auto dfa = dfa_builder.buildDFA(*nfa);
                dfas.push_back(std::move(dfa));
            } catch (const std::exception& e) {
                reportError("Failed to build DFA for token '" + pattern->getName() + "': " + e.what());
                return false;
            }
        }
        
        // Use CodeGenerator to generate all files
        CodeGenerator code_gen(patterns_, dfas, config_.class_name);
        auto generated_files = code_gen.generateAll();
        
        // Write token types header
        std::string token_types_file = config_.getTokenTypesFilePath();
        if (!writeFile(token_types_file, generated_files.token_types_hpp)) {
            reportError("Failed to write token types file: " + token_types_file);
            return false;
        }
        
        // Write token types implementation
        std::string token_types_impl_file = config_.output_directory + "/TokenTypes.cpp";
        if (!writeFile(token_types_impl_file, generated_files.token_types_cpp)) {
            reportError("Failed to write token types implementation file: " + token_types_impl_file);
            return false;
        }
        
        // Write DFA tables
        std::string dfa_tables_file = config_.output_directory + "/DFATables.hpp";
        if (!writeFile(dfa_tables_file, generated_files.dfa_tables_hpp)) {
            reportError("Failed to write DFA tables file: " + dfa_tables_file);
            return false;
        }
        
        std::cout << "Generated token types file: " << token_types_file << std::endl;
        std::cout << "Generated token types implementation: " << token_types_impl_file << std::endl;
        std::cout << "Generated DFA tables: " << dfa_tables_file << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        reportError("Failed to generate token types: " + std::string(e.what()));
        return false;
    }
}

bool LexerGenerator::generateLexerClass() {
    try {
        // Build NFAs and DFAs for all patterns (if not already built)
        std::cout << "Building NFAs and DFAs for " << patterns_.size() << " token patterns..." << std::endl;
        
        RegexParser regex_parser;
        NFABuilder nfa_builder;
        DFABuilder dfa_builder;
        
        std::vector<std::unique_ptr<DFA>> dfas;
        dfas.reserve(patterns_.size());
        
        for (const auto& pattern : patterns_) {
            try {
                auto ast = regex_parser.parse(pattern->getRegexPattern());
                auto nfa = nfa_builder.buildNFA(ast);
                auto dfa = dfa_builder.buildDFA(*nfa);
                dfas.push_back(std::move(dfa));
            } catch (const std::exception& e) {
                reportError("Failed to build DFA for token '" + pattern->getName() + "': " + e.what());
                return false;
            }
        }
        
        // Use CodeGenerator to generate lexer files
        CodeGenerator code_gen(patterns_, dfas, config_.class_name);
        auto generated_files = code_gen.generateAll();
        
        // Generate lexer header
        std::string header_file = config_.getHeaderFilePath();
        if (!writeFile(header_file, generated_files.lexer_hpp)) {
            reportError("Failed to write lexer header file: " + header_file);
            return false;
        }
        
        // Generate lexer implementation
        std::string impl_file = config_.getSourceFilePath();
        if (!writeFile(impl_file, generated_files.lexer_cpp)) {
            reportError("Failed to write lexer implementation file: " + impl_file);
            return false;
        }
        
        std::cout << "Generated lexer files:" << std::endl;
        std::cout << "  Header: " << header_file << std::endl;
        std::cout << "  Implementation: " << impl_file << std::endl;
        
        return true;
        
    } catch (const std::exception& e) {
        reportError("Failed to generate lexer class: " + std::string(e.what()));
        return false;
    }
}

bool LexerGenerator::updateMakefile() {
    try {
        std::string makefile_path = "Makefile";
        
        // Read existing Makefile
        std::ifstream makefile(makefile_path);
        if (!makefile.is_open()) {
            reportError("Could not open Makefile for updating");
            return false;
        }
        
        std::stringstream buffer;
        buffer << makefile.rdbuf();
        std::string makefile_content = buffer.str();
        makefile.close();
        
        // Check if Makefile already has lexer generator integration
        if (makefile_content.find("LEXER_GENERATOR") != std::string::npos) {
            std::cout << "Makefile already has lexer generator integration" << std::endl;
            return true;
        }
        
        // Add lexer generator integration to Makefile
        std::string integration = R"(
# === LEXER GENERATOR INTEGRATION ===
LEXER_GENERATOR := lexer_generator
TOKEN_FILE := src/lexer/hulk_language.tokens
GENERATED_LEXER := $(BUILD_DIR)/generated/GeneratedLexer.hpp
GENERATED_TOKEN_TYPES := $(BUILD_DIR)/generated/GeneratedTokenTypes.hpp

# Generate lexer from .tokens file
$(GENERATED_LEXER) $(GENERATED_TOKEN_TYPES): $(TOKEN_FILE) $(LEXER_GENERATOR)
	@./$(LEXER_GENERATOR) $(TOKEN_FILE) --output $(BUILD_DIR)/generated

# Build lexer generator
$(LEXER_GENERATOR): src/lexer_generator/main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# Include generated files in main build
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(GENERATED_LEXER) $(GENERATED_TOKEN_TYPES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) -c $< -o $@

# === END LEXER GENERATOR INTEGRATION ===
)";
        
        // Insert integration before the last target
        size_t last_target_pos = makefile_content.find_last_of("$(EXEC):");
        if (last_target_pos != std::string::npos) {
            makefile_content.insert(last_target_pos, integration);
        } else {
            // If no target found, append to end
            makefile_content += integration;
        }
        
        // Write updated Makefile
        if (!writeFile(makefile_path, makefile_content)) {
            reportError("Failed to write updated Makefile");
            return false;
        }
        
        std::cout << "Updated Makefile with lexer generator integration" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        reportError("Failed to update Makefile: " + std::string(e.what()));
        return false;
    }
}

void LexerGenerator::reportError(const std::string& message) {
    errors_.push_back(message);
    std::cerr << "Error: " << message << std::endl;
}

bool LexerGenerator::validatePatterns() const {
    if (patterns_.empty()) {
        // Use a temporary error collection for const method
        std::cerr << "Error: No token patterns defined" << std::endl;
        return false;
    }
    
    // Check for duplicate token names
    std::set<std::string> token_names;
    for (const auto& pattern : patterns_) {
        if (token_names.find(pattern->getName()) != token_names.end()) {
            std::cerr << "Error: Duplicate token name: " << pattern->getName() << std::endl;
            return false;
        }
        token_names.insert(pattern->getName());
    }
    
    // Check for empty patterns
    for (const auto& pattern : patterns_) {
        if (pattern->getRegexPattern().empty()) {
            std::cerr << "Error: Empty regex pattern for token: " << pattern->getName() << std::endl;
            return false;
        }
    }
    
    return true;
}

void LexerGenerator::sortPatternsByPriority() {
    std::sort(patterns_.begin(), patterns_.end(), 
        [](const std::shared_ptr<TokenPattern>& a, const std::shared_ptr<TokenPattern>& b) {
            return a->getPriority() > b->getPriority();
        });
}

std::string LexerGenerator::generateTokenTypeHeader() const {
    std::stringstream content;
    
    content << "#pragma once\n\n";
    content << "/**\n";
    content << " * @brief Auto-generated TokenType enum\n";
    content << " * Generated from .token file by LexerGenerator\n";
    content << " */\n\n";
    
    // Add includes if needed
    if (std::any_of(patterns_.begin(), patterns_.end(), 
                     [](const auto& p) { return p->hasDataType(); })) {
        content << "#include <string>\n";
        content << "#include <variant>\n\n";
    }
    
    // Generate TokenType enum
    content << "enum class TokenType {\n";
    
    for (size_t i = 0; i < patterns_.size(); ++i) {
        content << "    " << patterns_[i]->getName() << ",\n";
    }
    
    content << "    END_OF_FILE,\n";
    content << "    UNKNOWN\n";
    content << "};\n\n";
    
    // Generate TokenValue variant if any tokens have data types
    if (std::any_of(patterns_.begin(), patterns_.end(), 
                     [](const auto& p) { return p->hasDataType(); })) {
        content << "/**\n";
        content << " * @brief Token value storage using std::variant\n";
        content << " */\n";
        content << "using TokenValue = std::variant<";
        
        std::vector<std::string> types;
        for (const auto& pattern : patterns_) {
            if (pattern->hasDataType()) {
                types.push_back(pattern->getCppTypeName());
            }
        }
        
        // Add common types
        types.push_back("std::string");
        types.push_back("int");
        types.push_back("double");
        
        // Remove duplicates
        std::sort(types.begin(), types.end());
        types.erase(std::unique(types.begin(), types.end()), types.end());
        
        for (size_t i = 0; i < types.size(); ++i) {
            content << types[i];
            if (i < types.size() - 1) content << ", ";
        }
        
        content << ">;\n\n";
    }
    
    // Generate SourceLocation structure
    content << "struct SourceLocation {\n";
    content << "    int line;\n";
    content << "    int column;\n";
    content << "};\n\n";
    
    // Generate Token structure
    content << "struct Token {\n";
    content << "    TokenType type;\n";
    content << "    std::string lexeme;\n";
    content << "    SourceLocation location;\n";
    
    if (std::any_of(patterns_.begin(), patterns_.end(), 
                     [](const auto& p) { return p->hasDataType(); })) {
        content << "    TokenValue value;\n\n";
        content << "    Token(TokenType type, std::string lexeme, SourceLocation loc)\n";
        content << "        : type(type), lexeme(std::move(lexeme)), location(loc), value() {}\n";
        content << "    \n";
        content << "    Token(TokenType type, std::string lexeme, SourceLocation loc, TokenValue val)\n";
        content << "        : type(type), lexeme(std::move(lexeme)), location(loc), value(std::move(val)) {}\n";
    } else {
        content << "\n";
        content << "    Token(TokenType type, std::string lexeme, SourceLocation loc)\n";
        content << "        : type(type), lexeme(std::move(lexeme)), location(loc) {}\n";
    }
    
    content << "};\n\n";
    
    // Generate tokenTypeToString function declaration
    content << "/**\n";
    content << " * @brief Convert TokenType to string representation\n";
    content << " * @param type TokenType to convert\n";
    content << " * @return String representation of the token type\n";
    content << " */\n";
    content << "std::string tokenTypeToString(TokenType type);\n";
    
    return content.str();
}

std::string LexerGenerator::generateTokenTypeToString() const {
    std::stringstream content;
    
    content << "#include \"GeneratedTokenTypes.hpp\"\n";
    content << "#include <unordered_map>\n\n";
    
    content << "std::string tokenTypeToString(TokenType type) {\n";
    content << "    static const std::unordered_map<TokenType, std::string> mapping = {\n";
    
    for (const auto& pattern : patterns_) {
        content << "        {TokenType::" << pattern->getName() << ", \"" << pattern->getName() << "\"},\n";
    }
    
    content << "        {TokenType::END_OF_FILE, \"END_OF_FILE\"},\n";
    content << "        {TokenType::UNKNOWN, \"UNKNOWN\"}\n";
    content << "    };\n\n";
    
    content << "    auto it = mapping.find(type);\n";
    content << "    return (it != mapping.end()) ? it->second : \"UNKNOWN\";\n";
    content << "}\n";
    
    return content.str();
}

std::string LexerGenerator::generateLexerHeader() const {
    std::stringstream content;
    
    content << "#pragma once\n\n";
    content << "#include \"GeneratedTokenTypes.hpp\"\n";
    content << "#include <string>\n";
    content << "#include <vector>\n";
    content << "#include <memory>\n\n";
    
    if (!config_.namespace_name.empty()) {
        content << "namespace " << config_.namespace_name << " {\n\n";
    }
    
    content << "/**\n";
    content << " * @brief Auto-generated lexer class\n";
    content << " * Generated from .token file by LexerGenerator\n";
    content << " */\n";
    content << "class " << config_.class_name << " {\n";
    content << "public:\n";
    content << "    /**\n";
    content << "     * @brief Constructor\n";
    content << "     * @param input Source code to tokenize\n";
    content << "     */\n";
    content << "    " << config_.class_name << "(const std::string& input);\n\n";
    
    content << "    /**\n";
    content << "     * @brief Get the next token from the input\n";
    content << "     * @return Next token or nullptr if end of input\n";
    content << "     */\n";
    content << "    std::shared_ptr<Token> nextToken();\n\n";
    
    content << "    /**\n";
    content << "     * @brief Check if there are any lexical errors\n";
    content << "     * @return true if errors occurred\n";
    content << "     */\n";
    content << "    bool hasErrors() const { return !errors_.empty(); }\n\n";
    
    content << "    /**\n";
    content << "     * @brief Get all lexical errors\n";
    content << "     * @return Vector of error messages\n";
    content << "     */\n";
    content << "    const std::vector<std::string>& getErrors() const { return errors_; }\n\n";
    
    content << "private:\n";
    content << "    std::string source_;                    ///< Source code to tokenize\n";
    content << "    size_t pos_;                            ///< Current position in source\n";
    content << "    int line_;                              ///< Current line number\n";
    content << "    int column_;                            ///< Current column number\n";
    content << "    std::vector<std::string> errors_;       ///< Collection of error messages\n\n";
    
    content << "    // Helper methods\n";
    content << "    char peek() const;\n";
    content << "    char get();\n";
    content << "    void advance(int count);\n";
    content << "    void skipWhitespace();\n";
    content << "    void reportError(const std::string& message);\n";
    content << "    bool isAlpha(char c) const;\n";
    content << "    bool isDigit(char c) const;\n";
    content << "    bool isAlnum(char c) const;\n";
    content << "};\n";
    
    if (!config_.namespace_name.empty()) {
        content << "\n} // namespace " << config_.namespace_name << "\n";
    }
    
    return content.str();
}

std::string LexerGenerator::generateLexerImplementation() const {
    std::stringstream content;
    
    content << "#include \"" << config_.class_name << ".hpp\"\n";
    content << "#include <cctype>\n";
    content << "#include <iostream>\n";
    content << "#include <unordered_map>\n\n";
    
    if (!config_.namespace_name.empty()) {
        content << "namespace " << config_.namespace_name << " {\n\n";
    }
    
    // Constructor
    content << config_.class_name << "::" << config_.class_name << "(const std::string& input)\n";
    content << "    : source_(input), pos_(0), line_(1), column_(1) {}\n\n";
    
    // Helper methods
    content << "char " << config_.class_name << "::peek() const {\n";
    content << "    return (pos_ < source_.size()) ? source_[pos_] : '\\0';\n";
    content << "}\n\n";
    
    content << "char " << config_.class_name << "::get() {\n";
    content << "    char c = peek();\n";
    content << "    if (c == '\\n') {\n";
    content << "        ++line_;\n";
    content << "        column_ = 1;\n";
    content << "    } else {\n";
    content << "        ++column_;\n";
    content << "    }\n";
    content << "    ++pos_;\n";
    content << "    return c;\n";
    content << "}\n\n";
    
    content << "void " << config_.class_name << "::advance(int count) {\n";
    content << "    for (int i = 0; i < count; ++i) get();\n";
    content << "}\n\n";
    
    content << "void " << config_.class_name << "::skipWhitespace() {\n";
    content << "    while (isspace(peek())) get();\n";
    content << "}\n\n";
    
    content << "void " << config_.class_name << "::reportError(const std::string& message) {\n";
    content << "    errors_.emplace_back(message);\n";
    content << "}\n\n";
    
    content << "bool " << config_.class_name << "::isAlpha(char c) const {\n";
    content << "    return std::isalpha(c) || c == '_';\n";
    content << "}\n\n";
    
    content << "bool " << config_.class_name << "::isDigit(char c) const {\n";
    content << "    return std::isdigit(c);\n";
    content << "}\n\n";
    
    content << "bool " << config_.class_name << "::isAlnum(char c) const {\n";
    content << "    return std::isalnum(c) || c == '_';\n";
    content << "}\n\n";
    
    // nextToken method - this will be a placeholder for now
    content << "std::shared_ptr<Token> " << config_.class_name << "::nextToken() {\n";
    content << "    // TODO: Implement actual token recognition logic\n";
    content << "    // This is a placeholder implementation\n";
    content << "    \n";
    content << "    skipWhitespace();\n";
    content << "    \n";
    content << "    if (pos_ >= source_.size()) {\n";
    content << "        return std::make_shared<Token>(TokenType::END_OF_FILE, \"\", SourceLocation{line_, column_});\n";
    content << "    }\n";
    content << "    \n";
    content << "    // Placeholder: return UNKNOWN token\n";
    content << "    char c = get();\n";
    content << "    return std::make_shared<Token>(TokenType::UNKNOWN, std::string(1, c), SourceLocation{line_, column_});\n";
    content << "}\n";
    
    if (!config_.namespace_name.empty()) {
        content << "\n} // namespace " << config_.namespace_name << "\n";
    }
    
    return content.str();
}

bool LexerGenerator::writeFile(const std::string& filename, const std::string& content) const {
    try {
        // Backup existing file if requested
        if (config_.backup_existing_files) {
            backupFile(filename);
        }
        
        // Ensure directory exists
        std::filesystem::path file_path(filename);
        std::filesystem::create_directories(file_path.parent_path());
        
        // Write file
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        file << content;
        file.close();
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error writing file '" << filename << "': " << e.what() << std::endl;
        return false;
    }
}

bool LexerGenerator::backupFile(const std::string& filename) const {
    try {
        if (!std::filesystem::exists(filename)) {
            return true; // No file to backup
        }
        
        std::filesystem::path file_path(filename);
        std::string backup_name = filename + ".backup";
        
        std::filesystem::copy_file(filename, backup_name, 
                                  std::filesystem::copy_options::overwrite_existing);
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Warning: Could not backup file '" << filename << "': " << e.what() << std::endl;
        return false;
    }
} 
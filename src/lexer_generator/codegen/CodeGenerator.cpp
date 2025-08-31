#include "CodeGenerator.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

CodeGenerator::CodeGenerator(const std::vector<std::shared_ptr<TokenPattern>>& tokens,
                           const std::vector<std::unique_ptr<DFA>>& dfas,
                           const std::string& class_name)
    : tokens_(tokens), dfas_(dfas), class_name_(class_name) {
    
    // Build token name to index mapping
    for (size_t i = 0; i < tokens_.size(); ++i) {
        token_name_to_index_[tokens_[i]->getName()] = static_cast<int>(i + 1); // +1 to avoid 0 (UNKNOWN)
    }
}

CodeGenerator::GeneratedFiles CodeGenerator::generateAll() {
    GeneratedFiles files;
    files.token_types_hpp = generateTokenTypesHeader();
    files.token_types_cpp = generateTokenTypesImplementation();
    files.lexer_hpp = generateLexerHeader();
    files.lexer_cpp = generateLexerImplementation();
    files.dfa_tables_hpp = generateDFATablesHeader();
    return files;
}

std::string CodeGenerator::generateTokenTypesHeader() {
    std::ostringstream oss;
    
    oss << "#pragma once\n";
    oss << "#include <string>\n\n";
    
    // Generate TokenType enum
    oss << "enum class TokenType {\n";
    oss << "    UNKNOWN = 0,\n";
    
    for (size_t i = 0; i < tokens_.size(); ++i) {
        oss << "    " << tokens_[i]->getName() << ",\n";
    }
    
    // Add special tokens that are not in the .token file but needed by the lexer
    oss << "    END_OF_FILE,\n";
    oss << "    ERROR\n";
    
    oss << "};\n\n";
    
    // Function declarations
    oss << "// Auto-generated functions from .token file\n";
    oss << "std::string tokenTypeToString(TokenType type);\n";
    oss << "TokenType stringToTokenType(const std::string& name);\n";
    oss << "bool isKeyword(const std::string& lexeme);\n";
    oss << "TokenType getKeywordType(const std::string& lexeme);\n\n";
    
    return oss.str();
}

std::string CodeGenerator::generateTokenTypesImplementation() {
    std::ostringstream oss;
    
    oss << "#include \"TokenTypes.hpp\"\n";
    oss << "#include <unordered_map>\n";
    oss << "#include <unordered_set>\n";
    oss << "#include <algorithm>\n\n";
    
    // Generate tokenTypeToString function
    oss << "std::string tokenTypeToString(TokenType type) {\n";
    oss << "    static const std::unordered_map<TokenType, std::string> mapping = {\n";
    
    for (const auto& token : tokens_) {
        oss << "        {TokenType::" << token->getName() << ", \"" << token->getName() << "\"},\n";
    }
    
    // Add special tokens
    oss << "        {TokenType::END_OF_FILE, \"$\"},\n";
    oss << "        {TokenType::ERROR, \"ERROR\"},\n";
    
    oss << "    };\n";
    oss << "    auto it = mapping.find(type);\n";
    oss << "    return (it != mapping.end()) ? it->second : \"UNKNOWN\";\n";
    oss << "}\n\n";
    
    // Generate stringToTokenType function
    oss << "TokenType stringToTokenType(const std::string& name) {\n";
    oss << "    static const std::unordered_map<std::string, TokenType> reverse_mapping = {\n";
    
    for (const auto& token : tokens_) {
        oss << "        {\"" << token->getName() << "\", TokenType::" << token->getName() << "},\n";
    }
    
    // Add special tokens
    oss << "        {\"$\", TokenType::END_OF_FILE},\n";
    oss << "        {\"ERROR\", TokenType::ERROR},\n";
    
    oss << "    };\n";
    oss << "    auto it = reverse_mapping.find(name);\n";
    oss << "    return (it != reverse_mapping.end()) ? it->second : TokenType::UNKNOWN;\n";
    oss << "}\n\n";
    
    // Generate keyword functions
    oss << generateKeywordMap();
    
    return oss.str();
}

std::string CodeGenerator::generateKeywordMap() {
    std::ostringstream oss;
    
    // Find keywords (tokens with plain alphabetic patterns)
    std::vector<std::pair<std::string, std::string>> keywords;
    for (const auto& token : tokens_) {
        const std::string& pattern = token->getRegexPattern();
        bool is_plain_keyword = !pattern.empty() && 
            std::all_of(pattern.begin(), pattern.end(), [](char c) {
                return std::isalpha(static_cast<unsigned char>(c));
            });
        if (is_plain_keyword) {
            keywords.emplace_back(pattern, token->getName());
        }
    }
    
    oss << "bool isKeyword(const std::string& lexeme) {\n";
    oss << "    static const std::unordered_set<std::string> keywords = {\n";
    for (const auto& [keyword, _] : keywords) {
        oss << "        \"" << keyword << "\",\n";
    }
    oss << "    };\n";
    oss << "    return keywords.find(lexeme) != keywords.end();\n";
    oss << "}\n\n";
    
    oss << "TokenType getKeywordType(const std::string& lexeme) {\n";
    oss << "    static const std::unordered_map<std::string, TokenType> keyword_map = {\n";
    for (const auto& [keyword, token_name] : keywords) {
        oss << "        {\"" << keyword << "\", TokenType::" << token_name << "},\n";
    }
    oss << "    };\n";
    oss << "    auto it = keyword_map.find(lexeme);\n";
    oss << "    return (it != keyword_map.end()) ? it->second : TokenType::ID;\n";
    oss << "}\n";
    
    return oss.str();
}

std::string CodeGenerator::generateLexerHeader() {
    std::ostringstream oss;
    
    oss << "#pragma once\n";
    oss << "#include \"TokenTypes.hpp\"\n";
    oss << "#include <string>\n";
    oss << "#include <vector>\n";
    oss << "#include <memory>\n\n";
    
    // Forward declarations
    oss << "struct SourceLocation {\n";
    oss << "    int line;\n";
    oss << "    int column;\n";
    oss << "};\n\n";
    
    oss << "struct Token {\n";
    oss << "    TokenType type;\n";
    oss << "    std::string lexeme;\n";
    oss << "    SourceLocation location;\n\n";
    oss << "    Token() : type(TokenType::UNKNOWN), lexeme(\"\"), location{0, 0} {}\n";
    oss << "    Token(TokenType type, std::string lexeme, SourceLocation loc)\n";
    oss << "        : type(type), lexeme(std::move(lexeme)), location(loc) {}\n";
    oss << "};\n\n";
    
    // Lexer class
    oss << "class " << class_name_ << " {\n";
    oss << "public:\n";
    oss << "    explicit " << class_name_ << "(const std::string& input);\n\n";
    oss << "    // Main lexing methods\n";
    oss << "    std::shared_ptr<Token> nextToken();\n";
    oss << "    std::vector<std::shared_ptr<Token>> tokenize();\n";
    oss << "    bool hasMoreTokens() const;\n\n";
    oss << "private:\n";
    oss << "    std::string input_;\n";
    oss << "    size_t position_;\n";
    oss << "    int line_;\n";
    oss << "    int column_;\n\n";
    oss << "    // DFA execution helpers\n";
    oss << "    struct DFAResult {\n";
    oss << "        int length;\n";
    oss << "        TokenType token_type;\n";
    oss << "        int priority;\n";
    oss << "    };\n\n";
    oss << "    DFAResult runAllDFAs(size_t start_pos);\n";
    oss << "    bool isWhitespace(char c) const;\n";
    oss << "    void skipWhitespace();\n";
    oss << "};\n";
    
    return oss.str();
}

std::string CodeGenerator::generateLexerImplementation() {
    std::ostringstream oss;
    
    oss << "#include \"" << class_name_ << ".hpp\"\n";
    oss << "#include \"DFATables.hpp\"\n";
    oss << "#include <cctype>\n";
    oss << "#include <algorithm>\n\n";
    
    // Constructor
    oss << class_name_ << "::" << class_name_ << "(const std::string& input)\n";
    oss << "    : input_(input), position_(0), line_(1), column_(1) {}\n\n";
    
    // Helper methods
    oss << "bool " << class_name_ << "::isWhitespace(char c) const {\n";
    oss << "    return std::isspace(static_cast<unsigned char>(c)) != 0;\n";
    oss << "}\n\n";
    
    oss << "void " << class_name_ << "::skipWhitespace() {\n";
    oss << "    while (position_ < input_.size() && isWhitespace(input_[position_])) {\n";
    oss << "        if (input_[position_] == '\\n') {\n";
    oss << "            line_++;\n";
    oss << "            column_ = 1;\n";
    oss << "        } else {\n";
    oss << "            column_++;\n";
    oss << "        }\n";
    oss << "        position_++;\n";
    oss << "    }\n";
    oss << "}\n\n";
    
    // DFA execution
    oss << class_name_ << "::DFAResult " << class_name_ << "::runAllDFAs(size_t start_pos) {\n";
    oss << "    DFAResult best_result = {-1, TokenType::UNKNOWN, -1000000};\n\n";
    
    for (size_t i = 0; i < dfas_.size(); ++i) {
        const auto& dfa = dfas_[i];
        const auto& token = tokens_[i];
        
        oss << "    // DFA for " << token->getName() << "\n";
        oss << "    {\n";
        oss << "        int state = " << token->getName() << "_DFA_START_STATE;\n";
        oss << "        int last_accept_len = -1;\n";
        oss << "        size_t pos = start_pos;\n\n";
        oss << "        while (state >= 0 && pos < input_.size()) {\n";
        oss << "            if (" << token->getName() << "_DFA_ACCEPT[state]) {\n";
        oss << "                last_accept_len = static_cast<int>(pos - start_pos);\n";
        oss << "            }\n";
        oss << "            char c = input_[pos];\n";
        oss << "            auto it = " << token->getName() << "_DFA_TRANSITIONS[state].find(c);\n";
        oss << "            if (it == " << token->getName() << "_DFA_TRANSITIONS[state].end()) break;\n";
        oss << "            state = it->second;\n";
        oss << "            pos++;\n";
        oss << "        }\n";
        oss << "        if (state >= 0 && " << token->getName() << "_DFA_ACCEPT[state]) {\n";
        oss << "            last_accept_len = static_cast<int>(pos - start_pos);\n";
        oss << "        }\n";
        oss << "        if (last_accept_len > best_result.length || \n";
        oss << "            (last_accept_len == best_result.length && " << token->getPriority() << " > best_result.priority)) {\n";
        oss << "            best_result = {last_accept_len, TokenType::" << token->getName() << ", " << token->getPriority() << "};\n";
        oss << "        }\n";
        oss << "    }\n\n";
    }
    
    oss << "    return best_result;\n";
    oss << "}\n\n";
    
    // Main lexing methods
    oss << "std::shared_ptr<Token> " << class_name_ << "::nextToken() {\n";
    oss << "    skipWhitespace();\n\n";
    oss << "    if (position_ >= input_.size()) {\n";
    oss << "        return std::make_shared<Token>(TokenType::END_OF_FILE, \"\", SourceLocation{line_, column_});\n";
    oss << "    }\n\n";
    oss << "    size_t start_pos = position_;\n";
    oss << "    int start_line = line_;\n";
    oss << "    int start_column = column_;\n\n";
    oss << "    // Run all DFAs to find longest match\n";
    oss << "    DFAResult result = runAllDFAs(position_);\n\n";
    oss << "    if (result.length > 0) {\n";
    oss << "        // Valid token found - consume the matched portion\n";
    oss << "        std::string lexeme = input_.substr(position_, result.length);\n";
    oss << "        TokenType token_type = result.token_type;\n\n";
    oss << "        // Handle keyword remapping\n";
    oss << "        if (token_type == TokenType::ID && isKeyword(lexeme)) {\n";
    oss << "            token_type = getKeywordType(lexeme);\n";
    oss << "        }\n\n";
    oss << "        position_ += result.length;\n";
    oss << "        column_ += result.length;\n\n";
    oss << "        return std::make_shared<Token>(token_type, lexeme, SourceLocation{start_line, start_column});\n";
    oss << "    } else {\n";
    oss << "        // No match - consume one character as ERROR\n";
    oss << "        std::string lexeme = input_.substr(position_, 1);\n";
    oss << "        position_++;\n";
    oss << "        column_++;\n";
    oss << "        return std::make_shared<Token>(TokenType::ERROR, lexeme, SourceLocation{start_line, start_column});\n";
    oss << "    }\n";
    oss << "}\n\n";
    
    oss << "std::vector<std::shared_ptr<Token>> " << class_name_ << "::tokenize() {\n";
    oss << "    std::vector<std::shared_ptr<Token>> tokens;\n";
    oss << "    while (hasMoreTokens()) {\n";
    oss << "        tokens.push_back(nextToken());\n";
    oss << "    }\n";
    oss << "    // Always add END_OF_FILE token at the end\n";
    oss << "    tokens.push_back(std::make_shared<Token>(TokenType::END_OF_FILE, \"\", SourceLocation{line_, column_}));\n";
    oss << "    return tokens;\n";
    oss << "}\n\n";
    
    oss << "bool " << class_name_ << "::hasMoreTokens() const {\n";
    oss << "    return position_ < input_.size();\n";
    oss << "}\n";
    
    return oss.str();
}

std::string CodeGenerator::generateDFATablesHeader() {
    std::ostringstream oss;
    
    oss << "#pragma once\n";
    oss << "#include <unordered_map>\n";
    oss << "#include <vector>\n\n";
    
    // Generate DFA tables for each token
    for (size_t i = 0; i < dfas_.size(); ++i) {
        const auto& dfa = dfas_[i];
        const auto& token = tokens_[i];
        
        oss << "// DFA tables for " << token->getName() << "\n";
        oss << "constexpr int " << token->getName() << "_DFA_START_STATE = " << dfa->getStartState() << ";\n\n";
        
        // Generate transition tables
        oss << "const std::unordered_map<char, int> " << token->getName() << "_DFA_TRANSITIONS[] = {\n";
        for (int state_id = 0; state_id < dfa->getStateCount(); ++state_id) {
            const DFAState* state = dfa->getState(state_id);
            if (state && !state->transitions.empty()) {
                oss << "    { // State " << state_id << "\n";
                for (const auto& [symbol, next_state] : state->transitions) {
                    oss << "        {'" << escapeString(std::string(1, symbol)) << "', " << next_state << "},\n";
                }
                oss << "    },\n";
            } else {
                oss << "    {}, // State " << state_id << " (no transitions)\n";
            }
        }
        oss << "};\n\n";
        
        // Generate accept table
        oss << "const bool " << token->getName() << "_DFA_ACCEPT[] = {\n";
        for (int state_id = 0; state_id < dfa->getStateCount(); ++state_id) {
            const DFAState* state = dfa->getState(state_id);
            oss << "    " << (state && state->is_accepting ? "true" : "false") << ", // State " << state_id << "\n";
        }
        oss << "};\n\n";
        
        // Generate action table
        oss << "const int " << token->getName() << "_DFA_ACTION[] = {\n";
        for (int state_id = 0; state_id < dfa->getStateCount(); ++state_id) {
            const DFAState* state = dfa->getState(state_id);
            oss << "    " << (state ? state->action_number : -1) << ", // State " << state_id << "\n";
        }
        oss << "};\n\n";
    }
    
    return oss.str();
}

std::string CodeGenerator::escapeString(const std::string& str) {
    std::string result;
    for (char c : str) {
        switch (c) {
            case '\\': result += "\\\\"; break;
            case '\'': result += "\\'"; break;
            case '\"': result += "\\\""; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result += c; break;
        }
    }
    return result;
} 
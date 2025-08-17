#include "TokenFileParser.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdexcept>

std::vector<std::shared_ptr<TokenPattern>> TokenFileParser::parseTokenFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open token file: " + filename);
    }
    
    std::vector<std::shared_ptr<TokenPattern>> patterns;
    std::string line;
    int line_number = 0;
    
    clearErrors();
    
    while (std::getline(file, line)) {
        line_number++;
        
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        try {
            auto pattern = parsePatternLine(line, line_number);
            if (pattern) {
                patterns.push_back(pattern);
            }
        } catch (const std::exception& e) {
            reportError(e.what(), line_number);
        }
    }
    
    if (hasErrors()) {
        std::stringstream error_msg;
        error_msg << "Parsing errors found in " << filename << ":\n";
        for (const auto& error : errors_) {
            error_msg << "  " << error << "\n";
        }
        throw std::runtime_error(error_msg.str());
    }
    
    return patterns;
}

std::shared_ptr<TokenPattern> TokenFileParser::parsePatternLine(const std::string& line, int line_number) {
    // Strip comments first
    std::string clean_line = stripComments(line);
    
    // Skip empty lines after comment stripping
    if (clean_line.empty()) {
        return nullptr;
    }
    
    // Split by ':' to separate token type from pattern (only first occurrence)
    size_t colon_pos = clean_line.find(':');
    if (colon_pos == std::string::npos) {
        throw std::runtime_error("Invalid format: expected 'TokenType : Pattern'");
    }
    
    std::string token_type = trim(clean_line.substr(0, colon_pos));
    std::string pattern_part = trim(clean_line.substr(colon_pos + 1));
    
    // Validate token type name
    if (token_type.empty()) {
        throw std::runtime_error("Token type name cannot be empty");
    }
    
    // Check if token type contains only valid characters
    for (char c : token_type) {
        if (!std::isalnum(c) && c != '_') {
            throw std::runtime_error("Token type name contains invalid character: " + std::string(1, c));
        }
    }
    
    // Parse the pattern part which may contain data type and priority
    // We need to be careful about splitting on '|' since it can be part of regex patterns
    std::string regex_pattern;
    std::optional<std::string> data_type = std::nullopt;
    int priority = 0;
    std::string action = "return " + token_type + ";";
    
    // First, check for data type specification (-> type)
    size_t arrow_pos = pattern_part.find("->");
    if (arrow_pos != std::string::npos) {
        std::string before_arrow = pattern_part.substr(0, arrow_pos);
        std::string after_arrow = pattern_part.substr(arrow_pos + 2);
        
        regex_pattern = trim(before_arrow);
        data_type = trim(after_arrow);
    } else {
        regex_pattern = pattern_part;
    }
    
    // Now check for priority specification (| priority)
    // We need to find the last '|' that's not part of a regex pattern
    // This is a simplified approach - we'll look for '|' followed by a number
    size_t priority_pos = regex_pattern.find_last_of('|');
    if (priority_pos != std::string::npos) {
        // Check if what follows looks like a priority number
        std::string after_pipe = regex_pattern.substr(priority_pos + 1);
        after_pipe = trim(after_pipe);
        
        // If it's just a number, it's a priority
        if (!after_pipe.empty() && std::all_of(after_pipe.begin(), after_pipe.end(), ::isdigit)) {
            std::string before_pipe = regex_pattern.substr(0, priority_pos);
            regex_pattern = trim(before_pipe);
            priority = parsePriority(after_pipe);
        }
        // Otherwise, the '|' is part of the regex pattern
    }
    
    // Validate regex pattern
    if (regex_pattern.empty()) {
        throw std::runtime_error("Regex pattern cannot be empty");
    }
    
    if (!isValidRegex(regex_pattern)) {
        reportError("Regex pattern may be invalid: " + regex_pattern, line_number);
    }
    
    // Create and return the TokenPattern
    return std::make_shared<TokenPattern>(token_type, regex_pattern, action, priority, data_type);
}

std::string TokenFileParser::stripComments(const std::string& line) const {
    size_t comment_pos = line.find('#');
    if (comment_pos != std::string::npos) {
        return line.substr(0, comment_pos);
    }
    return line;
}

std::string TokenFileParser::trim(const std::string& str) const {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> TokenFileParser::split(const std::string& str, char delimiter) const {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    
    while (std::getline(ss, item, delimiter)) {
        result.push_back(trim(item));
    }
    
    return result;
}

bool TokenFileParser::isValidRegex(const std::string& pattern) const {
    // Basic regex validation - check for balanced parentheses and brackets
    int paren_count = 0;
    int bracket_count = 0;
    bool in_bracket = false;
    bool escape_next = false;
    
    for (size_t i = 0; i < pattern.length(); i++) {
        char c = pattern[i];
        
        if (escape_next) {
            escape_next = false;
            continue;
        }
        
        if (c == '\\') {
            escape_next = true;
            continue;
        }
        
        if (c == '(' && !in_bracket) {
            paren_count++;
        } else if (c == ')' && !in_bracket) {
            paren_count--;
            if (paren_count < 0) return false;
        } else if (c == '[' && !in_bracket) {
            bracket_count++;
            in_bracket = true;
        } else if (c == ']' && in_bracket) {
            bracket_count--;
            in_bracket = false;
            if (bracket_count < 0) return false;
        }
    }
    
    return paren_count == 0 && bracket_count == 0 && !in_bracket;
}

void TokenFileParser::reportError(const std::string& message, int line_number) {
    std::stringstream error_msg;
    error_msg << "Line " << line_number << ": " << message;
    errors_.push_back(error_msg.str());
}

int TokenFileParser::parsePriority(const std::string& priority_str) const {
    try {
        return std::stoi(priority_str);
    } catch (const std::exception&) {
        // If parsing fails, return default priority
        return 0;
    }
}

std::string TokenFileParser::parseDataType(const std::string& type_str) const {
    // Normalize common type names
    std::string normalized = type_str;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
    
    if (normalized == "int" || normalized == "integer") {
        return "int";
    } else if (normalized == "double" || normalized == "float" || normalized == "number") {
        return "double";
    } else if (normalized == "string" || normalized == "str") {
        return "std::string";
    } else if (normalized == "bool" || normalized == "boolean") {
        return "bool";
    } else if (normalized == "char") {
        return "char";
    } else {
        // Return as-is for custom types
        return type_str;
    }
} 
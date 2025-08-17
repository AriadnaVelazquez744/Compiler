#include "TokenPattern.hpp"
#include <stdexcept>

TokenPattern::TokenPattern(const std::string& name, 
                         const std::string& regex_pattern, 
                         const std::string& action, 
                         int priority,
                         const std::optional<std::string>& data_type)
    : name_(name), regex_pattern_(regex_pattern), action_(action), 
      priority_(priority), data_type_(data_type) {
    
    // Validate input parameters
    if (name.empty()) {
        throw std::invalid_argument("Token name cannot be empty");
    }
    
    if (regex_pattern.empty()) {
        throw std::invalid_argument("Regex pattern cannot be empty");
    }
    
    if (action.empty()) {
        throw std::invalid_argument("Action cannot be empty");
    }
    
    // Validate priority range
    if (priority < 0) {
        throw std::invalid_argument("Priority must be non-negative");
    }
}

std::string TokenPattern::getCppTypeName() const {
    if (!data_type_.has_value()) {
        return ""; // No data type specified
    }
    
    // Map common type names to C++ types
    if (data_type_ == "int" || data_type_ == "integer") {
        return "int";
    } else if (data_type_ == "double" || data_type_ == "float" || data_type_ == "number") {
        return "double";
    } else if (data_type_ == "string" || data_type_ == "str") {
        return "std::string";
    } else if (data_type_ == "bool" || data_type_ == "boolean") {
        return "bool";
    } else if (data_type_ == "char") {
        return "char";
    } else {
        // Return as-is for custom types
        return data_type_.value();
    }
}

std::string TokenPattern::getDefaultValue() const {
    if (!data_type_.has_value()) {
        return ""; // No default value for untyped tokens
    }
    
    std::string cpp_type = getCppTypeName();
    
    if (cpp_type == "int") {
        return "0";
    } else if (cpp_type == "double") {
        return "0.0";
    } else if (cpp_type == "std::string") {
        return "\"\"";
    } else if (cpp_type == "bool") {
        return "false";
    } else if (cpp_type == "char") {
        return "'\\0'";
    } else {
        // For custom types, try to provide a reasonable default
        return "{}";
    }
} 
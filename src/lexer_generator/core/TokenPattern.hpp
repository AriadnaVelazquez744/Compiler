#pragma once

#include <string>
#include <optional>

/**
 * @brief Represents a token pattern definition from a .token file
 * 
 * This class holds all the information needed to generate a token:
 * - name: The token type name (e.g., "NUMBER", "IDENTIFIER")
 * - regex_pattern: The regex pattern to match this token
 * - action: The action to execute when this token is matched
 * - priority: Priority for disambiguation (higher = more specific)
 * - data_type: Optional C++ data type for the token value
 */
class TokenPattern {
public:
    /**
     * @brief Constructor for TokenPattern
     * @param name Token type name
     * @param regex_pattern Regex pattern to match
     * @param action Action code to execute
     * @param priority Priority for disambiguation
     * @param data_type Optional C++ data type
     */
    TokenPattern(const std::string& name, 
                 const std::string& regex_pattern, 
                 const std::string& action, 
                 int priority = 0,
                 const std::optional<std::string>& data_type = std::nullopt);

    // Getters
    const std::string& getName() const { return name_; }
    const std::string& getRegexPattern() const { return regex_pattern_; }
    const std::string& getAction() const { return action_; }
    int getPriority() const { return priority_; }
    const std::optional<std::string>& getDataType() const { return data_type_; }
    
    // Setters
    void setPriority(int priority) { priority_ = priority; }
    void setDataType(const std::string& data_type) { data_type_ = data_type; }
    
    /**
     * @brief Check if this pattern has a data type
     * @return true if data type is specified
     */
    bool hasDataType() const { return data_type_.has_value(); }
    
    /**
     * @brief Get the C++ type name for this token
     * @return C++ type name or empty string if no type specified
     */
    std::string getCppTypeName() const;
    
    /**
     * @brief Get the default value for this token's data type
     * @return Default value string for C++ code generation
     */
    std::string getDefaultValue() const;

private:
    std::string name_;                    ///< Token type name (e.g., "NUMBER")
    std::string regex_pattern_;           ///< Regex pattern to match
    std::string action_;                  ///< Action code to execute
    int priority_;                        ///< Priority for disambiguation
    std::optional<std::string> data_type_; ///< Optional C++ data type
}; 
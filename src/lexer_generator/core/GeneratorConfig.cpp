#include "GeneratorConfig.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>

GeneratorConfig::GeneratorConfig() {
    // Constructor already initializes all members with default values
}

bool GeneratorConfig::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open config file: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Parse key=value pairs
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            // Set configuration values
            if (key == "output_directory") {
                output_directory = value;
            } else if (key == "class_name") {
                class_name = value;
            } else if (key == "namespace_name") {
                namespace_name = value;
            } else if (key == "generate_tests") {
                generate_tests = (value == "true" || value == "1");
            } else if (key == "optimize_dfa") {
                optimize_dfa = (value == "true" || value == "1");
            } else if (key == "compress_tables") {
                compress_tables = (value == "true" || value == "1");
            } else if (key == "strict_mode") {
                strict_mode = (value == "true" || value == "1");
            }
        }
    }
    
    return true;
}

bool GeneratorConfig::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not create config file: " << filename << std::endl;
        return false;
    }
    
    file << "# Lexer Generator Configuration File\n";
    file << "# Generated automatically\n\n";
    
    file << "output_directory=" << output_directory << "\n";
    file << "class_name=" << class_name << "\n";
    file << "namespace_name=" << namespace_name << "\n";
    file << "generate_tests=" << (generate_tests ? "true" : "false") << "\n";
    file << "optimize_dfa=" << (optimize_dfa ? "true" : "false") << "\n";
    file << "compress_tables=" << (compress_tables ? "true" : "false") << "\n";
    file << "strict_mode=" << (strict_mode ? "true" : "false") << "\n";
    
    return true;
}

bool GeneratorConfig::isValid() const {
    // Check if required fields are set
    if (class_name.empty()) {
        std::cerr << "Error: Class name cannot be empty" << std::endl;
        return false;
    }
    
    if (output_directory.empty()) {
        std::cerr << "Error: Output directory cannot be empty" << std::endl;
        return false;
    }
    
    // Check if output directory is writable
    if (!ensureOutputDirectory()) {
        std::cerr << "Error: Cannot create or access output directory: " << output_directory << std::endl;
        return false;
    }
    
    return true;
}

std::string GeneratorConfig::getHeaderFilePath() const {
    return output_directory + "/" + header_file;
}

std::string GeneratorConfig::getSourceFilePath() const {
    return output_directory + "/" + source_file;
}

std::string GeneratorConfig::getTokenTypesFilePath() const {
    return output_directory + "/" + token_types_file;
}

bool GeneratorConfig::ensureOutputDirectory() const {
    try {
        std::filesystem::path dir(output_directory);
        if (!std::filesystem::exists(dir)) {
            return std::filesystem::create_directories(dir);
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error creating output directory: " << e.what() << std::endl;
        return false;
    }
} 
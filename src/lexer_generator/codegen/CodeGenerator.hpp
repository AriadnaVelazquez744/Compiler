#pragma once
#include <vector>
#include <string>
#include <memory>
#include <map>
#include "../core/TokenPattern.hpp"
#include "../automata/DFA.hpp"

class CodeGenerator {
public:
    struct GeneratedFiles {
        std::string token_types_hpp;
        std::string token_types_cpp;
        std::string lexer_hpp;
        std::string lexer_cpp;
        std::string dfa_tables_hpp;
    };

    CodeGenerator(const std::vector<std::shared_ptr<TokenPattern>>& tokens,
                  const std::vector<std::unique_ptr<DFA>>& dfas,
                  const std::string& class_name = "GeneratedLexer");

    // Generate all files
    GeneratedFiles generateAll();

    // Individual file generators
    std::string generateTokenTypesHeader();
    std::string generateTokenTypesImplementation();
    std::string generateLexerHeader();
    std::string generateLexerImplementation();
    std::string generateDFATablesHeader();

private:
    const std::vector<std::shared_ptr<TokenPattern>>& tokens_;
    const std::vector<std::unique_ptr<DFA>>& dfas_;
    std::string class_name_;
    std::map<std::string, int> token_name_to_index_;
    
    // Helper methods
    std::string generateDFAStateTable(const DFA& dfa, const std::string& token_name);
    std::string generateDFAAcceptTable(const DFA& dfa, const std::string& token_name);
    std::string generateDFAActionTable(const DFA& dfa, const std::string& token_name);
    std::string generateKeywordMap();
    std::string generateTokenTypeToStringMap();
    std::string generateStringToTokenTypeMap();
    std::string escapeString(const std::string& str);
}; 
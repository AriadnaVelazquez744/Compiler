// CodeGenerator.hpp
#pragma once

#include "../core/GrammarAugment.hpp"
#include "../core/LR1ItemSetBuilder.hpp"
#include "../core/LR1ParsingTables.hpp"
#include <string>
#include <fstream>

class CodeGenerator {
public:
    CodeGenerator(const GrammarAugment& grammar, 
                  const LR1ItemSetBuilder& itemBuilder,
                  const LR1ParsingTableGenerator& tableGen);

    // Generate all parser component files
    void generateAllFiles(const std::string& outputDir = "generated_parser");

    // Individual file generation methods
    void generateGrammarProductions(const std::string& filename);
    void generateFirstSets(const std::string& filename);
    void generateFollowSets(const std::string& filename);
    void generateLR1ItemSets(const std::string& filename);
    void generateParsingTables(const std::string& filename);

private:
    const GrammarAugment& grammar;
    const LR1ItemSetBuilder& itemBuilder;
    const LR1ParsingTableGenerator& tableGen;

    // Helper methods for file generation
    void writeHeader(std::ofstream& file, const std::string& title);
    void writeFooter(std::ofstream& file);
    std::string escapeString(const std::string& str);
    std::string formatProduction(const std::string& lhs, const std::vector<std::string>& rhs);
}; 
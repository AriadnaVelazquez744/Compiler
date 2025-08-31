// main_codegen.cpp
// Main driver for the Code Generator
// This file demonstrates how to use the CodeGenerator to generate
// all parser component files

#include "CodeGenerator.hpp"
#include "../core/GrammarAugment.hpp"
#include "../core/LR1ItemSetBuilder.hpp"
#include "../core/LR1ParsingTables.hpp"
#include "../grammar/PrecedenceSetup.hpp"
#include <iostream>

int main() {
    std::cout << "=== Parser Code Generator ===\n";
    std::cout << "This program generates separate files for each parser component\n\n";

    try {
        // 1. Load and process grammar
        std::cout << "1. Loading grammar...\n";
        GrammarAugment grammar;
        
        // Use the correct relative path from the codegen directory
        std::string grammarFile = "../grammar/BNFGrammar.bnf";
        std::cout << "   Loading grammar from: " << grammarFile << std::endl;
        
        grammar.readGrammar(grammarFile);
        
        // Check if grammar was loaded successfully
        if (grammar.getProductions().empty()) {
            std::cerr << "Error: Grammar file could not be loaded or is empty\n";
            std::cerr << "Please check the file path: " << grammarFile << std::endl;
            return 1;
        }
        
        std::cout << "   Grammar loaded successfully\n";
        std::cout << "   Productions loaded: " << grammar.getProductions().size() << " non-terminals\n";

        // 2. Compute First and Follow sets
        std::cout << "2. Computing First and Follow sets...\n";
        grammar.computeFirstSets();
        grammar.computeFollowSets();
        std::cout << "   First and Follow sets computed\n";

        // 3. Build LR(1) item sets
        std::cout << "3. Building LR(1) item sets...\n";
        LR1ItemSetBuilder itemBuilder(grammar);
        itemBuilder.constructItemSets();
        std::cout << "   LR(1) item sets constructed\n";

        // 4. Generate parsing tables
        std::cout << "4. Generating parsing tables...\n";
        LR1ParsingTableGenerator tableGen(grammar, itemBuilder);
        
        // Setup precedence (if available)
        try {
            setupPrecedence(tableGen);
            std::cout << "   Precedence setup applied\n";
        } catch (...) {
            std::cout << "   Precedence setup skipped (not available)\n";
        }
        
        tableGen.generateParsingTables();
        std::cout << "   Parsing tables generated\n";

        // 5. Generate code files
        std::cout << "5. Generating code files...\n";
        CodeGenerator codeGen(grammar, itemBuilder, tableGen);
        codeGen.generateAllFiles("../.build");
        
        std::cout << "\n=== Code Generation Complete ===\n";
        std::cout << "Generated files:\n";
        std::cout << "  - src/parser/.build/grammar_productions.cpp\n";
        std::cout << "  - src/parser/.build/first_sets.cpp\n";
        std::cout << "  - src/parser/.build/follow_sets.cpp\n";
        std::cout << "  - src/parser/.build/lr1_item_sets.cpp\n";
        std::cout << "  - src/parser/.build/parsing_tables.cpp\n\n";
        
        std::cout << "These files contain all the data structures needed\n";
        std::cout << "to process token lists during parsing.\n";

    } catch (const std::exception& e) {
        std::cerr << "Error during code generation: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

#include "lexer/Lexer.hpp"
#include "parser/GrammarAugment.hpp"
#include "parser/LR1ItemSetBuilder.hpp"
#include "parser/LR1ParsingTables.hpp"
#include "parser/ParserDriver.hpp"
#include "parser/SemanticActionDispatcher.hpp"
#include "parser/PrecedenceSetup.hpp"
#include "parser/TokenTypeStringMap.hpp"
#include "ast/AST.hpp"
// #include "semantic/SemanticAnalyzer.hpp"
// #include "codegen/CodeGenContext.hpp"

#include <filesystem> // C++17

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

    // 1. LEXER: Convert source code into tokens
    Lexer lexer(source);
    std::vector<std::shared_ptr<Token>> tokens;
    while (true) {
        auto token = lexer.nextToken();
        tokens.push_back(token);
        std::cerr << "Token encontrado: " << tokenTypeToString(token->type) << " => " << token->lexeme << "\n";
        if (token->type == TokenType::END_OF_FILE) break;
    }

    if (!lexer.getErrors().empty()) {
        std::cerr << "Errores léxicos encontrados:\n";
        for (const auto& err : lexer.getErrors()) {
            std::cerr << "  Línea " << err.location.line << ", Columna " 
                      << err.location.column << ": " << err.message << "\n";
        }
        return 1;
    }

    // 2. LOAD GRAMMAR AND BUILD PARSER
    GrammarAugment grammar;

    // std::string grammarPath = std::filesystem::current_path().string() + "parser/BNFGrammar.bnf";
    try {
        grammar.readGrammar("src/parser/BNFGrammar.bnf");

    } catch (const std::exception& e) {
        std::cerr << "Error al cargar la gramática: " << e.what() << std::endl;
        return 1;
    }

    grammar.computeFirstSets();
    grammar.computeFollowSets();
    std::cerr << "conjuntos first y follow definidos \n";

    LR1ItemSetBuilder itemBuilder(grammar);
    itemBuilder.constructItemSets();
    std::cerr << "conjuntos LR(1) definidos \n";

    LR1ParsingTableGenerator tableGen(grammar, itemBuilder);
    setupPrecedence(tableGen); // External config
    tableGen.generateParsingTables();
    std::cerr << "tablas action and gotoc y precedencia establecida \n";

    tableGen.printGotoTable();

    // 3. PARSE TOKENS → AST
    SemanticActionDispatcher dispatcher;
    std::cerr << "action dispatcher inicialyze \n";

    ParserDriver driver(tableGen, dispatcher);
    std::cerr << "parser driver initialize \n";

    ParseResult result = driver.parse(tokens);
    std::cerr << "parse result generated \n";

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

    std::cout << "AST generado exitosamente.\n";
    for (const auto& node : result.ast) {
        std::cout << "Tipo de nodo raíz: " << node->type() 
                  << " | Línea: " << node->line() << "\n";
    }

    // // 4. SEMANTIC ANALYSIS (future)
    // SemanticAnalyzer analyzer;
    // analyzer.analyze(result.ast);

    // std::cout << "Análisis semántico completado.\n";

    // // 5. CODEGEN (optional)
    // CodeGenContext codegen;
    // try {
    //     codegen.generateCode(result.ast);
    // } catch (const std::exception& e) {
    //     std::cerr << "Error en generación de código: " << e.what() << "\n";
    //     return 1;
    // }

    // codegen.dumpIR("hulk-low-code.ll");
    // std::cout << "IR volcado en 'hulk-low-code.ll'.\n";

    return 0;
}

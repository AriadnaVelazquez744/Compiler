#include "../src/parser/ParserDriver.hpp"
#include "../src/ast/AST.hpp"
#include "../src/parser/grammar/SemanticActionProvider.hpp"
#include "../src/parser/core/GrammarAugment.hpp"
#include "../src/parser/core/LR1ItemSetBuilder.hpp"
#include "../src/parser/core/LR1ParsingTables.hpp"
#include <ostream>
#include <iostream>

// Dummy semantic action provider
class DummySemanticProvider : public SemanticActionProvider {
public:
    std::shared_ptr<ASTNode> apply(
        int prodIndex,
        const std::vector<std::shared_ptr<ASTNode>>& children,
        const Token& /*lookahead*/)
    {
        return std::make_shared<LiteralNode>(
            "R" + std::to_string(prodIndex),
            "dummy",
            children.empty() ? 0 : children.front()->line());
    }
};

int main() {
    GrammarAugment grammar;
    grammar.readGrammar("src/parser/BNFGrammar.bnf");
    grammar.computeFirstSets();
    grammar.computeFollowSets();

    LR1ItemSetBuilder builder(grammar);
    builder.constructItemSets();

    LR1ParsingTableGenerator tableGen(grammar, builder);
    tableGen.generateParsingTables();

    // Dummy lexer with fixed token stream
    class DummyLexer : public Lexer {
    public:
        DummyLexer() : Lexer("") {
            tokens = {
                std::make_shared<Token>(TokenType::NUMBER, "NUMBER", SourceLocation{1, 1}),
                std::make_shared<Token>(TokenType::ADD, "ADD", SourceLocation{1, 2}),
                std::make_shared<Token>(TokenType::ID, "ID", SourceLocation{1, 3}),
                std::make_shared<Token>(TokenType::SEMICOLON, "SEMICOLON", SourceLocation{1, 4}),
                std::make_shared<Token>(TokenType::END_OF_FILE, "$", SourceLocation{1, 5}),
            };
        }

        std::shared_ptr<Token> nextToken() {
            if (index < tokens.size())
                return tokens[index++];
            return std::make_shared<Token>(TokenType::END_OF_FILE, "$", SourceLocation{0, 0});
        }

    private:
        size_t index = 0;
        std::vector<std::shared_ptr<Token>> tokens;
    };

    DummyLexer lexer;
    auto actionProvider = std::make_shared<DummySemanticProvider>();
    Parser parser(lexer, tableGen, grammar, actionProvider);

    try {
        auto results = parser.parse();
        std::cout << "Parsed " << results.size() << " node(s).\n";
        for (auto& node : results) {
            std::cout << "  -> " << node->type() << " at line " << node->line() << "\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "Parser error: " << ex.what() << std::endl;
    }

    return 0;
}

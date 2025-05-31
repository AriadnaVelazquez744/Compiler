// GrammarNormalizer.hpp

#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <unordered_map>

struct RawRule {
    std::string lhs;
    std::string rhsExpr;
};

struct BNFRule {
    std::string lhs;
    std::vector<std::string> rhs;
};

enum class EBNFNodeType {
    SYMBOL, SEQUENCE, ALTERNATION, ZERO_OR_MORE, ONE_OR_MORE, OPTIONAL
};

struct EBNFNode {
    EBNFNodeType type;
    std::vector<std::shared_ptr<EBNFNode>> children;
    std::string symbol;

    EBNFNode(EBNFNodeType t) : type(t) {}
    EBNFNode(const std::string& sym) : type(EBNFNodeType::SYMBOL), symbol(sym) {}
};

class GrammarNormalizer {
public:
    GrammarNormalizer();

    // Main entry: normalize all raw rules and write BNF to file
    void normalizeRulesToFile(const std::vector<RawRule>& rules, const std::string& outPath);

private:
    int anonCounter;

    std::string freshNonTerminal(const std::string& base);

    std::shared_ptr<EBNFNode> parseRHS(const std::string& rhsExpr);
    std::shared_ptr<EBNFNode> parseEBNFExpr();
    std::shared_ptr<EBNFNode> parseSequence();
    std::shared_ptr<EBNFNode> parseTerm();
    std::shared_ptr<EBNFNode> parseAtom();

    void tokenize(const std::string& rhs);
    std::string peek();
    std::string get();
    bool match(const std::string& tok);
    bool isSymbol(const std::shared_ptr<EBNFNode>& s);

    std::string flattenEBNF(std::shared_ptr<EBNFNode> node, std::vector<BNFRule>& outRules, bool forceCreateRule = false);

    void writeBNFToFile(const std::vector<BNFRule>& rules, const std::string& outPath);

    std::vector<std::string> tokens;
    size_t tokPos;
};

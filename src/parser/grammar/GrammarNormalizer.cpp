// GrammarNormalizer.cpp
#include "GrammarNormalizer.hpp"
#include <sstream>
#include <iostream>
#include <cctype>
#include <stdexcept>

GrammarNormalizer::GrammarNormalizer() : anonCounter(0), tokPos(0) {}

// Generate fresh non-terminal names
std::string GrammarNormalizer::freshNonTerminal(const std::string& base) {
    return base + "_gen_" + std::to_string(anonCounter++);
}

// ======== TOKENIZER ========
void GrammarNormalizer::tokenize(const std::string& rhs) {
    tokens.clear();
    tokPos = 0;
    std::string buffer;

    auto flushBuffer = [&]() {
        if (!buffer.empty()) {
            tokens.push_back(buffer);
            buffer.clear();
        }
    };

    for (char c : rhs) {
        if (isspace(c)) {
            flushBuffer();
        } else if (c == '(' || c == ')' || c == '*' || c == '+' || c == '?' || c == '|') {
            flushBuffer();
            tokens.emplace_back(1, c);
        } else {
            buffer += c;
        }
    }

    flushBuffer();
}

// ======== PARSER ========
std::shared_ptr<EBNFNode> GrammarNormalizer::parseRHS(const std::string& rhsExpr) {
    tokenize(rhsExpr);
    return parseEBNFExpr();
}

std::shared_ptr<EBNFNode> GrammarNormalizer::parseEBNFExpr() {
    std::vector<std::shared_ptr<EBNFNode>> alts;
    alts.push_back(parseSequence());

    while (match("|")) {
        alts.push_back(parseSequence());
    }

    if (alts.size() == 1) {
        return alts[0];
    }

    auto altNode = std::make_shared<EBNFNode>(EBNFNodeType::ALTERNATION);
    altNode->children = std::move(alts);
    return altNode;
}

std::shared_ptr<EBNFNode> GrammarNormalizer::parseSequence() {
    auto seq = std::make_shared<EBNFNode>(EBNFNodeType::SEQUENCE);
    while (tokPos < tokens.size() && tokens[tokPos] != ")" && tokens[tokPos] != "|") {
        seq->children.push_back(parseTerm());
    }

    if (seq->children.size() == 1)
        return seq->children[0];

    return seq;
}

std::shared_ptr<EBNFNode> GrammarNormalizer::parseTerm() {
    auto atom = parseAtom();

    if (match("*")) {
        auto rep = std::make_shared<EBNFNode>(EBNFNodeType::ZERO_OR_MORE);
        rep->children.push_back(atom);
        return rep;
    } else if (match("+")) {
        auto rep = std::make_shared<EBNFNode>(EBNFNodeType::ONE_OR_MORE);
        rep->children.push_back(atom);
        return rep;
    } else if (match("?")) {
        auto opt = std::make_shared<EBNFNode>(EBNFNodeType::OPTIONAL);
        opt->children.push_back(atom);
        return opt;
    }

    return atom;
}

std::shared_ptr<EBNFNode> GrammarNormalizer::parseAtom() {
    if (match("(")) {
        auto expr = parseEBNFExpr();
        if (!match(")")) {
            throw std::runtime_error("Missing closing ')'");
        }
        return expr;
    }

    std::string sym = get();
    return std::make_shared<EBNFNode>(sym);
}

std::string GrammarNormalizer::peek() {
    if (tokPos < tokens.size()) return tokens[tokPos];
    return "";
}

std::string GrammarNormalizer::get() {
    return (tokPos < tokens.size()) ? tokens[tokPos++] : "";
}

bool GrammarNormalizer::match(const std::string& tok) {
    if (peek() == tok) {
        ++tokPos;
        return true;
    }
    return false;
}

bool GrammarNormalizer::isSymbol(const std::shared_ptr<EBNFNode>& node) {
    return node->type == EBNFNodeType::SYMBOL;
}

// ======== NORMALIZATION ========
std::string GrammarNormalizer::flattenEBNF(std::shared_ptr<EBNFNode> node, std::vector<BNFRule>& outRules, bool forceCreateRule) {

    switch (node->type) {
        case EBNFNodeType::SYMBOL:
            return node->symbol;

        case EBNFNodeType::SEQUENCE: {
            std::vector<std::string> rhs;
            for (auto& child : node->children) {
                std::string part = flattenEBNF(child, outRules, forceCreateRule);
                if (!part.empty()) {
                    rhs.push_back(part);
                }
            }
            
            if (rhs.empty()) return "";

            if (rhs.size() == 1) {
                return rhs[0];
            }

            if (!forceCreateRule) {
                return "";
            }

            std::string lhs = freshNonTerminal("seq");
            outRules.push_back(BNFRule{lhs, rhs});
            return lhs;
        }

        case EBNFNodeType::ALTERNATION: {
            std::string lhs = freshNonTerminal("alt");
            for (auto& option : node->children) {
                std::string sym = flattenEBNF(option, outRules, true);
                if (!sym.empty()) {
                    outRules.push_back(BNFRule{lhs, {sym}});
                }
            }
            return lhs;
        }

        case EBNFNodeType::ZERO_OR_MORE:
        case EBNFNodeType::ONE_OR_MORE: {
            bool isPlus = (node->type == EBNFNodeType::ONE_OR_MORE);
            std::string repType = isPlus ? "plus" : "star";

            std::shared_ptr<EBNFNode> repeated = node->children[0];
            std::vector<std::string> repeatedRHS;

            if (repeated->type == EBNFNodeType::SEQUENCE) {
                for (auto& child : repeated->children) {
                    std::string part = flattenEBNF(child, outRules, true);
                    if (!part.empty()) repeatedRHS.push_back(part);
                }
            } else {
                std::string part = flattenEBNF(repeated, outRules, true);
                if (!part.empty()) repeatedRHS.push_back(part);
            }

            std::string lhs = freshNonTerminal(repType);
            if (!isPlus) {
                outRules.push_back(BNFRule{lhs, {}});
            }

            if (!repeatedRHS.empty()) {
                std::vector<std::string> recursive = repeatedRHS;
                recursive.push_back(lhs);
                outRules.push_back(BNFRule{lhs, recursive});
                if (isPlus) {
                    outRules.push_back(BNFRule{lhs, repeatedRHS});
                }
            }

            return lhs;
        }

        case EBNFNodeType::OPTIONAL: {
            std::string item = flattenEBNF(node->children[0], outRules, true);
            std::string lhs = freshNonTerminal("opt");
            outRules.push_back(BNFRule{lhs, {}});
            if (!item.empty()) {
                outRules.push_back(BNFRule{lhs, {item}});
            }
            return lhs;
        }

        default:
            throw std::runtime_error("Unknown EBNF node type in flattenEBNF()");
    }
}

// ======== FILE WRITING ========
void GrammarNormalizer::writeBNFToFile(const std::vector<BNFRule>& rules, const std::string& outPath) {
    std::ofstream out(outPath);
    if (!out) {
        throw std::runtime_error("Failed to open output file: " + outPath);
    }

    for (const auto& rule : rules) {
        out << rule.lhs << " :";
        for (const auto& sym : rule.rhs) {
            out << " " << sym;
        }
        if (rule.rhs.empty()) {
            out << " ε";
        }
        out << "\n";
    }
    out.close();
}

// ======== PUBLIC ENTRY POINT ========
void GrammarNormalizer::normalizeRulesToFile(const std::vector<RawRule>& rules, const std::string& outPath) {
    std::vector<BNFRule> allRules;

    for (const auto& rule : rules) {
        auto ast = parseRHS(rule.rhsExpr);
        std::vector<std::string> rhs;

        if (ast->type == EBNFNodeType::SEQUENCE) {
            for (auto& child : ast->children) {
                std::string part = flattenEBNF(child, allRules);
                if (!part.empty()) {
                    rhs.push_back(part);
                }
            }

        } else {
            rhs.push_back(flattenEBNF(ast, allRules));
        }

        allRules.push_back(BNFRule{rule.lhs, rhs});

    }

    writeBNFToFile(allRules, outPath);
}

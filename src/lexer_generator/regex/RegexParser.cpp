#include "RegexParser.hpp"
#include <stdexcept>
#include <cctype>

std::shared_ptr<RegexNode> RegexParser::parse(const std::string& pattern) {
    clearErrors();
    input_ = pattern;
    pos_ = 0;
    length_ = input_.length();
    
    try {
        auto result = parseExpression();
        
        if (!isAtEnd()) {
            reportError("Unexpected characters at end of pattern");
        }
        
        if (hasErrors()) {
            throw std::runtime_error("Regex parsing failed");
        }
        
        return result;
    } catch (const std::exception& e) {
        reportError(e.what());
        throw;
    }
}

std::shared_ptr<RegexNode> RegexParser::parseExpression() {
    // expression = term ('|' term)*
    auto left = parseTerm();
    
    while (peek() == '|') {
        get(); // consume '|'
        auto right = parseTerm();
        
        auto alternation = std::make_shared<AlternationNode>();
        alternation->addChild(left);
        alternation->addChild(right);
        left = alternation;
    }
    
    return left;
}

std::shared_ptr<RegexNode> RegexParser::parseTerm() {
    // term = factor*
    std::vector<std::shared_ptr<RegexNode>> factors;
    
    while (!isAtEnd() && peek() != '|' && peek() != ')') {
        factors.push_back(parseFactor());
    }
    
    if (factors.empty()) {
        return std::make_shared<EpsilonNode>();
    }
    
    if (factors.size() == 1) {
        return factors[0];
    }
    
    // Create concatenation node
    auto concatenation = std::make_shared<ConcatenationNode>();
    concatenation->setChildren(std::move(factors));
    return concatenation;
}

std::shared_ptr<RegexNode> RegexParser::parseFactor() {
    // factor = atom ('*' | '+' | '?' | '{n,m}')?
    auto atom = parseAtom();
    
    if (isAtEnd()) {
        return atom;
    }
    
    char c = peek();
    if (c == '*' || c == '+' || c == '?') {
        get(); // consume quantifier
        
        RepetitionType type;
        int min_count = 0, max_count = -1;
        
        switch (c) {
            case '*':
                type = RepetitionType::ZERO_OR_MORE;
                break;
            case '+':
                type = RepetitionType::ONE_OR_MORE;
                break;
            case '?':
                type = RepetitionType::ZERO_OR_ONE;
                break;
        }
        
        auto repetition = std::make_shared<RepetitionNode>(type, min_count, max_count);
        repetition->addChild(atom);
        return repetition;
    }
    
    if (c == '{') {
        return parseRepetition(atom);
    }
    
    return atom;
}

std::shared_ptr<RegexNode> RegexParser::parseAtom() {
    char c = peek();
    
    if (c == '(') {
        return parseGroup();
    }
    
    if (c == '[') {
        return parseCharacterClass();
    }
    
    if (c == '\\') {
        get(); // consume backslash
        if (isAtEnd()) {
            reportError("Unexpected end after backslash");
            return std::make_shared<CharacterNode>('\\');
        }
        char escaped = get();
        return std::make_shared<CharacterNode>(unescapeChar(escaped));
    }
    
    if (c == '.' || c == '^' || c == '$') {
        get(); // consume special character
        if (c == '.') {
            // Dot matches any character
            auto charClass = std::make_shared<CharacterClassNode>(false);
            charClass->addRange(0, 127); // ASCII range
            return charClass;
        } else if (c == '^') {
            return std::make_shared<AnchorNode>(AnchorNode::AnchorType::START_OF_LINE);
        } else if (c == '$') {
            return std::make_shared<AnchorNode>(AnchorNode::AnchorType::END_OF_LINE);
        }
    }
    
    if (isSpecialChar(c)) {
        get(); // consume special character
        return std::make_shared<CharacterNode>(c);
    }
    
    // Regular character
    get();
    return std::make_shared<CharacterNode>(c);
}

std::shared_ptr<RegexNode> RegexParser::parseCharacterClass() {
    get(); // consume '['
    
    bool negated = false;
    if (peek() == '^') {
        negated = true;
        get(); // consume '^'
    }
    
    auto charClass = std::make_shared<CharacterClassNode>(negated);
    
    while (!isAtEnd() && peek() != ']') {
        char c = get();
        
        if (c == '\\') {
            if (isAtEnd()) {
                reportError("Unexpected end after backslash in character class");
                break;
            }
            char escaped = get();
            charClass->addCharacter(unescapeChar(escaped));
        } else if (c == '-' && !charClass->getCharacters().empty()) {
            // Range: a-z
            char start = charClass->getCharacters().back();
            // Note: We need to modify the character class, so we'll handle this differently
            // For now, just add the characters as-is
            charClass->addCharacter('-');
            charClass->addCharacter(start);
            
            if (isAtEnd() || peek() == ']') {
                reportError("Invalid range in character class");
                charClass->addCharacter('-');
                charClass->addCharacter(start);
            } else {
                char end = get();
                charClass->addRange(start, end);
            }
        } else {
            charClass->addCharacter(c);
        }
    }
    
    if (isAtEnd()) {
        reportError("Unterminated character class");
    } else {
        get(); // consume ']'
    }
    
    return charClass;
}

std::shared_ptr<RegexNode> RegexParser::parseRepetition(std::shared_ptr<RegexNode> atom) {
    get(); // consume '{'
    
    std::string min_str, max_str;
    bool has_comma = false;
    
    while (!isAtEnd() && peek() != '}' && peek() != ',') {
        if (!std::isdigit(peek())) {
            reportError("Invalid repetition count");
            break;
        }
        min_str += get();
    }
    
    if (peek() == ',') {
        has_comma = true;
        get(); // consume ','
        
        while (!isAtEnd() && peek() != '}') {
            if (!std::isdigit(peek())) {
                reportError("Invalid repetition count");
                break;
            }
            max_str += get();
        }
    }
    
    if (isAtEnd() || peek() != '}') {
        reportError("Unterminated repetition");
        return atom;
    }
    
    get(); // consume '}'
    
    int min_count = min_str.empty() ? 0 : std::stoi(min_str);
    int max_count = max_str.empty() ? -1 : std::stoi(max_str);
    
    if (min_count < 0 || (max_count != -1 && max_count < min_count)) {
        reportError("Invalid repetition range");
        return atom;
    }
    
    RepetitionType type;
    if (min_count == 0 && max_count == -1) {
        type = RepetitionType::ZERO_OR_MORE;
    } else if (min_count == 1 && max_count == -1) {
        type = RepetitionType::ONE_OR_MORE;
    } else if (min_count == 0 && max_count == 1) {
        type = RepetitionType::ZERO_OR_ONE;
    } else {
        type = RepetitionType::RANGE;
    }
    
    auto repetition = std::make_shared<RepetitionNode>(type, min_count, max_count);
    repetition->addChild(atom);
    return repetition;
}

std::shared_ptr<RegexNode> RegexParser::parseGroup() {
    get(); // consume '('
    
    auto group = std::make_shared<GroupNode>();
    group->addChild(parseExpression());
    
    if (isAtEnd() || peek() != ')') {
        reportError("Unterminated group");
    } else {
        get(); // consume ')'
    }
    
    return group;
}

// Helper methods
char RegexParser::peek() const {
    return pos_ < length_ ? input_[pos_] : '\0';
}

char RegexParser::get() {
    return pos_ < length_ ? input_[pos_++] : '\0';
}

void RegexParser::advance() {
    if (pos_ < length_) pos_++;
}

bool RegexParser::isAtEnd() const {
    return pos_ >= length_;
}

bool RegexParser::match(char expected) {
    if (peek() == expected) {
        get();
        return true;
    }
    return false;
}

void RegexParser::skipWhitespace() {
    while (std::isspace(peek())) {
        get();
    }
}

void RegexParser::reportError(const std::string& message) {
    std::string error = "Position " + std::to_string(pos_) + ": " + message;
    errors_.push_back(error);
}

bool RegexParser::isSpecialChar(char c) const {
    return c == '*' || c == '+' || c == '?' || c == '{' || c == '}' ||
           c == '[' || c == ']' || c == '(' || c == ')' || c == '|' ||
           c == '\\' || c == '.' || c == '^' || c == '$';
}

bool RegexParser::isQuantifier(char c) const {
    return c == '*' || c == '+' || c == '?' || c == '{';
}

char RegexParser::unescapeChar(char c) const {
    switch (c) {
        case 'n': return '\n';
        case 't': return '\t';
        case 'r': return '\r';
        case 'f': return '\f';
        case 'v': return '\v';
        case 'a': return '\a';
        case 'b': return '\b';
        case 's': return ' ';
        case 'd': return 'd'; // digit class placeholder
        case 'w': return 'w'; // word class placeholder
        case 'S': return 'S'; // non-space class placeholder
        case 'D': return 'D'; // non-digit class placeholder
        case 'W': return 'W'; // non-word class placeholder
        default: return c;
    }
} 
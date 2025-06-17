#include "ParserDriver.hpp"
#include "TokenTypeStringMap.hpp"
#include <iostream>

ParserDriver::ParserDriver(const LR1ParsingTableGenerator& tableGen, SemanticActionDispatcher& dispatcher)
    : tableGen(tableGen), dispatcher(dispatcher), currentTokenIndex(0) {}

ParseResult ParserDriver::parse(const std::vector<std::shared_ptr<Token>>& tokens) {
    std::cout << "\n=== Starting Parser ===" << std::endl;
    // Initialize stacks
    while (!valueStack.empty()) valueStack.pop();
    while (!stateStack.empty()) stateStack.pop();
    errors.clear();
    currentTokenIndex = 0;

    // Push initial state
    stateStack.push(0);
    std::cout << "Initial state pushed: 0" << std::endl;

    while (true) {
        if (currentTokenIndex >= tokens.size()) {
            std::cout << "Reached end of input" << std::endl;
            // Try to reduce any remaining items
            int currentState = stateStack.top();
            const auto& actionMap = tableGen.getActionTable(currentState);
            auto actionIt = actionMap.find("$");
            
            if (actionIt != actionMap.end() && actionIt->second.type == ActionType::Reduce) {
                const auto& action = actionIt->second;
                int prodNum = action.target;
                int rhsLength = dispatcher.getRHSLength(prodNum);
                std::string lhs = dispatcher.getLHS(prodNum);

                if (lhs == "S'") {
                    std::cout << "Accepting program" << std::endl;
                    return {dispatcher.getRootNodes(), errors};
                }
                
                std::cout << "Final reduction by production " << prodNum << ": " << lhs << " ::= ";
                for (int i = 0; i < rhsLength; i++) {
                    std::cout << "RHS[" << i << "] ";
                }
                std::cout << std::endl;
                
                // Pop RHS elements from stacks
                std::vector<ParserValue> children;
                for (int i = 0; i < rhsLength; i++) {
                    if (valueStack.empty()) {
                        std::cout << "ERROR: Value stack empty during reduction" << std::endl;
                        errors.push_back("Internal parser error: Stack underflow during reduction");
                        return {std::vector<std::shared_ptr<ASTNode>>{}, errors};
                    }
                    children.insert(children.begin(), valueStack.top());
                    valueStack.pop();
                    stateStack.pop();
                }
                
                // Get source location from first child
                SourceLocation loc;
                if (!children.empty()) {
                    if (auto token = std::get_if<std::shared_ptr<Token>>(&children[0])) {
                        loc = (*token)->location;
                    } else if (auto node = std::get_if<std::shared_ptr<ASTNode>>(&children[0])) {
                        loc = SourceLocation{(*node)->line(), 0};
                    }
                }
                
                // Perform reduction
                auto result = dispatcher.reduce(prodNum, children, loc);
                if (!result) {
                    return {dispatcher.getRootNodes(), errors};
                }
                std::cout << "Reduction result: " << (result ? "Node created" : "null") << std::endl;
                
                // Handle statement reduction if this is a statement
                if (lhs == "stmt") {
                    handleStatementReduction(tokens);
                }
                
                valueStack.push(result);
                const auto& gotoTable = tableGen.getGotoTable(stateStack.top());
                auto gotoIt = gotoTable.find(lhs);
                if (gotoIt != gotoTable.end()) {
                    int newState = gotoIt->second;
                    std::cout << "Goto state " << newState << " for " << lhs << std::endl;
                    stateStack.push(newState);
                    
                    // Check if we can accept
                    const auto& newActionMap = tableGen.getActionTable(newState);
                    auto acceptIt = newActionMap.find("$");
                    if (acceptIt != newActionMap.end() && acceptIt->second.type == ActionType::Accept) {
                        std::cout << "=== Parser Complete ===" << std::endl;
                        return handleAccept();
                    }
                }
            }
            break;
        }

        int currentState = stateStack.top();
        const auto& currentToken = tokens[currentTokenIndex];
        
        // Check if current token is a semicolon
        if (isSEMICOLON(currentToken)) {
            std::cout << "Found semicolon, attempting reduction" << std::endl;
            const auto& actionMap = tableGen.getActionTable(currentState);
            auto actionIt = actionMap.find("$");
            
            if (actionIt != actionMap.end() && actionIt->second.type == ActionType::Reduce) {
                const auto& action = actionIt->second;
                int prodNum = action.target;
                int rhsLength = dispatcher.getRHSLength(prodNum);
                std::string lhs = dispatcher.getLHS(prodNum);

                if (lhs == "S'") {
                    std::cout << "Accepting program" << std::endl;
                    return {dispatcher.getRootNodes(), errors};
                }
                
                std::cout << "Reducing by production " << prodNum << ": " << lhs << " ::= ";
                for (int i = 0; i < rhsLength; i++) {
                    std::cout << "RHS[" << i << "] ";
                }
                std::cout << std::endl;
                
                // Pop RHS elements from stacks
                std::vector<ParserValue> children;
                for (int i = 0; i < rhsLength; i++) {
                    if (valueStack.empty()) {
                        std::cout << "ERROR: Value stack empty during reduction" << std::endl;
                        errors.push_back("Internal parser error: Stack underflow during reduction");
                        return {std::vector<std::shared_ptr<ASTNode>>{}, errors};
                    }
                    children.insert(children.begin(), valueStack.top());
                    valueStack.pop();
                    stateStack.pop();
                }
                
                // Get source location from first child
                SourceLocation loc;
                if (!children.empty()) {
                    if (auto token = std::get_if<std::shared_ptr<Token>>(&children[0])) {
                        loc = (*token)->location;
                    } else if (auto node = std::get_if<std::shared_ptr<ASTNode>>(&children[0])) {
                        loc = SourceLocation{(*node)->line(), 0};
                    }
                }
                
                // Perform reduction
                auto result = dispatcher.reduce(prodNum, children, loc);
                if (!result) {
                    return {dispatcher.getRootNodes(), errors};
                }
                std::cout << "Reduction result: " << (result ? "Node created" : "null") << std::endl;
                
                // Handle statement reduction if this is a statement
                if (lhs == "stmt") {
                    handleStatementReduction(tokens);
                }
                
                valueStack.push(result);
                const auto& gotoTable = tableGen.getGotoTable(stateStack.top());
                auto gotoIt = gotoTable.find(lhs);
                if (gotoIt != gotoTable.end()) {
                    int newState = gotoIt->second;
                    std::cout << "Goto state " << newState << " for " << lhs << std::endl;
                    stateStack.push(newState);
                }
                
                // Skip the semicolon
                currentTokenIndex++;
                continue;
            }
        }
        
        // Normal token processing
        const auto& actionMap = tableGen.getActionTable(currentState);
        std::string tokenType = tokenTypeToString(currentToken->type);
        if ((currentTokenIndex >= tokens.size()) && tokenType == "$") {
            std::cout << "Accepting program" << std::endl;
            return {dispatcher.getRootNodes(), errors};
        }
        std::cout << "\nCurrent State: " << currentState << std::endl;
        std::cout << "Current Token: " << tokenType << " => " << currentToken->lexeme << std::endl;
        
        auto actionIt = actionMap.find(tokenType);
        if (actionIt == actionMap.end()) {
            std::cout << "ERROR: No action found for token " << tokenType << std::endl;
            const auto& expected = tableGen.getExpectedTokens(currentState);
            reportError(currentToken, expected);
            handleError(tokens);
            continue;
        }

        const auto& action = actionIt->second;
        std::cout << "Action Type: " << (action.type == ActionType::Shift ? "Shift" : 
                                       action.type == ActionType::Reduce ? "Reduce" :
                                       action.type == ActionType::Accept ? "Accept" : "Error") << std::endl;
        
        switch (action.type) {
            case ActionType::Shift: {
                std::cout << "Shifting to state: " << action.target << std::endl;
                stateStack.push(action.target);
                valueStack.push(currentToken);
                currentTokenIndex++;
                break;
            }
            
            case ActionType::Reduce: {
                int prodNum = action.target;
                int rhsLength = dispatcher.getRHSLength(prodNum);
                std::string lhs = dispatcher.getLHS(prodNum);

                if (lhs == "S'") {
                    std::cout << "Accepting program" << std::endl;
                    return {dispatcher.getRootNodes(), errors};
                }
                
                std::cout << "Reducing by production " << prodNum << ": " << lhs << " ::= ";
                for (int i = 0; i < rhsLength; i++) {
                    std::cout << "RHS[" << i << "] ";
                }
                std::cout << std::endl;
                
                std::vector<ParserValue> children;
                for (int i = 0; i < rhsLength; i++) {
                    if (valueStack.empty()) {
                        std::cout << "ERROR: Value stack empty during reduction" << std::endl;
                        errors.push_back("Internal parser error: Stack underflow during reduction");
                        return {std::vector<std::shared_ptr<ASTNode>>{}, errors};
                    }
                    children.insert(children.begin(), valueStack.top());
                    valueStack.pop();
                    stateStack.pop();
                }
                
                SourceLocation loc;
                if (!children.empty()) {
                    if (auto token = std::get_if<std::shared_ptr<Token>>(&children[0])) {
                        loc = (*token)->location;
                    } else if (auto node = std::get_if<std::shared_ptr<ASTNode>>(&children[0])) {
                        loc = SourceLocation{(*node)->line(), 0};
                    }
                }
                
                auto result = dispatcher.reduce(prodNum, children, loc);
                if (!result) {
                    return {dispatcher.getRootNodes(), errors};
                }
                std::cout << "Reduction result: " << (result ? "Node created" : "null") << std::endl;
                
                // Handle statement reduction if this is a statement
                if (lhs == "stmt") {
                    handleStatementReduction(tokens);
                }
                
                valueStack.push(result);
                const auto& gotoTable = tableGen.getGotoTable(stateStack.top());
                auto gotoIt = gotoTable.find(lhs);
                if (gotoIt != gotoTable.end()) {
                    int newState = gotoIt->second;
                    std::cout << "Goto state " << newState << " for " << lhs << std::endl;
                    stateStack.push(newState);
                }
                break;
            }
            
            case ActionType::Accept:
                std::cout << "=== Parser Complete ===" << std::endl;
                return handleAccept();
                
            case ActionType::Error:
                std::cout << "Error action encountered" << std::endl;
                handleError(tokens);
                continue;
        }
    }

    std::cout << "Reached end of input without accept" << std::endl;
    errors.push_back("Unexpected end of input");
    return {std::vector<std::shared_ptr<ASTNode>>{}, errors};
}

void ParserDriver::reportError(const std::shared_ptr<Token>& token, const std::set<std::string>& expected) {
    std::string errorMsg = "Error en línea " + std::to_string(token->location.line) + 
                          ": Token inesperado '" + token->lexeme + "' (" + 
                          tokenTypeToString(token->type) + "). ";
    
    if (!expected.empty()) {
        errorMsg += "Se esperaba uno de: ";
        for (const auto& exp : expected) {
            errorMsg += exp + ", ";
        }
        errorMsg = errorMsg.substr(0, errorMsg.length() - 2);
    }
    
    errors.push_back(errorMsg);
}

void ParserDriver::skipToNextSemicolon(const std::vector<std::shared_ptr<Token>>& tokens) {
    while (currentTokenIndex < tokens.size() && 
           !isSEMICOLON(tokens[currentTokenIndex]) && 
           !isRBRACE(tokens[currentTokenIndex])) {
        currentTokenIndex++;
    }
    
    // Skip all consecutive semicolons
    while (currentTokenIndex < tokens.size() && isSEMICOLON(tokens[currentTokenIndex])) {
        currentTokenIndex++;
    }
}

bool ParserDriver::isRBRACE(const std::shared_ptr<Token>& token) const {
    return token->type == TokenType::RBRACE;
}

bool ParserDriver::isSEMICOLON(const std::shared_ptr<Token>& token) const {
    return token->type == TokenType::SEMICOLON;
}

void ParserDriver::handleStatementReduction(const std::vector<std::shared_ptr<Token>>& tokens) {
    // Check if we're at the end of input
    if (currentTokenIndex >= tokens.size()) {
        errors.push_back("Error: Unexpected end of input after statement");
        return;
    }

    // 1. Check if current token is a semicolon
    bool hasSemicolon = isSEMICOLON(tokens[currentTokenIndex]);

    if (hasSemicolon) {
        // 2. Skip all consecutive semicolons
        while (currentTokenIndex < tokens.size() && isSEMICOLON(tokens[currentTokenIndex])) {
            currentTokenIndex++;
        }
        // Move back one position to keep currentToken at the last semicolon
        currentTokenIndex--;
        return;
    }

    // 3. Check if previous or next token is RBRACE
    bool hasRBrace = false;
    if (currentTokenIndex > 0 && isRBRACE(tokens[currentTokenIndex - 1])) {
        hasRBrace = true;
    }
    if (currentTokenIndex < tokens.size() - 1 && isRBRACE(tokens[currentTokenIndex + 1])) {
        hasRBrace = true;
    }

    if (hasRBrace) {
        return;
    }

    // 4. If not semicolon and no RBRACE, report error
    errors.push_back("Error en línea " + std::to_string(tokens[currentTokenIndex]->location.line) + 
                    ": Se esperaba ';' después de la declaración");
}

ParseResult ParserDriver::handleAccept() {
    return {dispatcher.getRootNodes(), errors};
}

void ParserDriver::handleError(const std::vector<std::shared_ptr<Token>>& tokens) {
    // Skip tokens until we find a semicolon or end of file
    skipToNextSemicolon(tokens);
    
    // If we've reached the end of the input, return
    if (currentTokenIndex >= tokens.size()) {
        return;
    }
    
    // Reset the parser state to a known good state
    while (stateStack.size() > 1) {
        stateStack.pop();
    }
    
    // Instead of clearing the value stack, we'll keep it
    // This preserves the valid tokens we've processed
    // while (!valueStack.empty()) {
    //     valueStack.pop();
    // }
    
    // Push the initial state back
    stateStack.push(0);
}

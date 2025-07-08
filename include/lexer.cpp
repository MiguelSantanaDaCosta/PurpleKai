// include/lexer.h
#pragma once

#include <string>
#include <vector>

enum class TokenType {
    // Palavras-chave
    CLASSE, FONCTION, LAISSER, CONSTANT,
    SI, SINON, POUR, TANTQUE,
    // ... outros tokens
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
};

class Lexer {
public:
    explicit Lexer(const std::string& source);
    std::vector<Token> tokenize();
    
private:
    const std::string& source_;
    size_t start_ = 0;
    size_t current_ = 0;
    size_t line_ = 1;
    
    char advance();
    bool match(char expected);
    void addToken(TokenType type);
    void scanToken();
    // ... outros métodos auxiliares
};

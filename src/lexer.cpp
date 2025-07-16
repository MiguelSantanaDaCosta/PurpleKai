#include "lexer.h"
#include <unordered_map>
#include <iostream>

Lexer::Lexer(const std::string& source) : source_(source) {}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        start_ = current_;
        char c = advance();

        if (std::isspace(c)) {
            if (c == '\n') line_++;
            continue;
        }

        if (std::isalpha(c) || c == '_') {
            identifier();
        } else if (std::isdigit(c)) {
            number();
        } else {
            switch (c) {
                case '+': addToken(TokenType::PLUS); break;
                case '-': addToken(TokenType::MOINS); break;
                case '*': addToken(TokenType::FOIS); break;
                case '/': addToken(TokenType::DIVISE); break;
                case '%': addToken(TokenType::PERCENT); break;
                case '=':
                    addToken(match('=') ? TokenType::EGAL_EGAL : TokenType::EGAL);
                    break;
                case '!':
                    addToken(match('=') ? TokenType::DIFFERENT : TokenType::NON);
                    break;
                case '<':
                    addToken(match('=') ? TokenType::INF_EGAL : TokenType::INFERIEUR);
                    break;
                case '>':
                    addToken(match('=') ? TokenType::SUP_EGAL : TokenType::SUPERIUER);
                    break;
                case '&':
                    if (match('&')) addToken(TokenType::ET);
                    break;
                case '|':
                    if (match('|')) addToken(TokenType::OU);
                    break;
                case '(': addToken(TokenType::LEFT_PAREN); break;
                case ')': addToken(TokenType::RIGHT_PAREN); break;
                case '{': addToken(TokenType::LEFT_BRACE); break;
                case '}': addToken(TokenType::RIGHT_BRACE); break;
                case '[': addToken(TokenType::CHOCHET_OUVRANT); break;
                case ']': addToken(TokenType::CHOCHET_FERMANT); break;
                case ',': addToken(TokenType::COMMA); break;
                case '.': addToken(TokenType::DOT); break;
                case ';': addToken(TokenType::SEMICOLON); break;
                case ':': addToken(TokenType::COLON); break;
                case '"': string(); break;

                default:
                    std::cerr << "Caractere inesperado na linha " << line_ << ": '" << c << "'\n";
                    break;
            }
        }
    }

    tokens_.push_back({TokenType::FIN_FICHIER, "", static_cast<int>(line_)});
    return tokens_;
}

bool Lexer::isAtEnd() const {
    return current_ >= source_.size();
}

char Lexer::advance() {
    return source_[current_++];
}

bool Lexer::match(char expected) {
    if (isAtEnd() || source_[current_] != expected) return false;
    current_++;
    return true;
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source_[current_];
}

char Lexer::peekNext() const {
    if (current_ + 1 >= source_.size()) return '\0';
    return source_[current_ + 1];
}

void Lexer::addToken(TokenType type) {
    std::string text = source_.substr(start_, current_ - start_);
    tokens_.push_back({type, text, static_cast<int>(line_)});
}

void Lexer::string() {
    while (!isAtEnd() && peek() != '"') {
        if (peek() == '\n') line_++;
        advance();
    }

    if (isAtEnd()) {
        std::cerr << "Erro: string não terminada na linha " << line_ << "\n";
        return;
    }

    advance(); // fecha "
    std::string value = source_.substr(start_ + 1, current_ - start_ - 2);
    tokens_.push_back({TokenType::CHAINE, value, static_cast<int>(line_)});
}

void Lexer::number() {
    while (std::isdigit(peek())) advance();

    if (peek() == '.' && std::isdigit(peekNext())) {
        advance();
        while (std::isdigit(peek())) advance();
        addToken(TokenType::DECIMAL);
    } else {
        addToken(TokenType::NOMBRE);
    }
}

void Lexer::identifier() {
    while (std::isalnum(peek()) || peek() == '_') advance();
    std::string text = source_.substr(start_, current_ - start_);

    auto it = keywords.find(text);
    TokenType type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIANT;
    tokens_.push_back({type, text, static_cast<int>(line_)});
}




//mapa de PALAVRAS-CHAVE
static const std::unordered_map<std::string, TokenType> Lexer::keywords ={

      {"classe", TokenType::CLASSE},
      {"fonction", TokenType::FONCTION},
      {"laisser", TokenType::LAISSER},
      {"constant", TokenType:: CONSTANTE},
      {"si", TokenType::SI},
      {"sinon", TokenType::SINON},
      {"pour", TokenType::POUR},
      {"tantque", TokenType::TANTQUE},
      {"retourner", TokenType::RETOURNER},
      {"nouveau", TokenType::NOUVEAU},
      {"vrai", TokenType::VRAI},
      {"faux", TokenType::FAUX},
      {"nulle", TokenType::NULLE},
      {"identifiant", TokenType::IDENTIFIANT},
      {"nombre", TokenType::NOMBRE},
      {"decimal", TokenType::DECIMAL},
      {"chaine", TokenType::CHAINE},
      {"plus", TokenType::PLUS},
      {"moins", TokenType::MOINS},
      {"fois", TokenType::FOIS},
      {"divise", TokenType::DIVISE},
      {"percent", TokenType::PERCENT},
      {"egal", TokenType::EGAL},
      {"egal_egal", TokenType::EGAL_EGAL},
      {"different", TokenType::DIFFERENT},
      {"inferieur", TokenType::INFERIEUR},
      {"inf_egal", TokenType:: INF_EGAL},
      {"superieur", TokenType::SUPERIEUR},
      {"sup_egal", TokenType::SUP_EGAL},
      {"et", TokenType::ET},
      {"ou", TokenType::OU},
      {"non", TokenType::NON},
      {"left_paren", TokenType::LEFT_PAREN},
      {"right_paren", TokenType::RIGHT_PAREN},
      {"left_brace", TokenType::LEFT_BRACE},
      {"right_brace", TokenType::RIGHT_BRACE},
      {"chochet_ouvrant", TokenType::CHOCHET_OUVRANT},
      {"chochet_fermant", TokenType::CHOCHET_FERMANT},
      {"comma", TokenType::COMMA},
      {"dot", TokenType::DOT},
      {"semicolon", TokenType::SEMICOLON},
      {"colon", TokenType::COLON},
      {"fin_fichier", TokenType::FIN_FICHIER}
};

    
     












 
};


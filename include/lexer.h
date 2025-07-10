// include/lexer.h
#pragma once

#include <string>
#include <vector>
#include <cctype>


enum class TokenType {
    // ==============================================
    // PALAVRAS-CHAVE (com equivalentes em C++)
    // ==============================================
    CLASSE,       // ≡ class (C++)
    FONCTION,     // ≡ (métodos em C++, sem equivalente exato para declaração)
    LAISSER,      // ≡ auto (mas com tipagem dinâmica)
    CONSTANTE,    // ≡ constexpr/const (dependendo do uso)
    
    // Controle de fluxo
    SI,           // ≡ if
    SINON,        // ≡ else
    POUR,         // ≡ for (mais próximo do range-based for do C++)
    TANTQUE,      // ≡ while
    RETOURNER,    // ≡ return
    
    // Valores e alocação
    NOUVEAU,      // ≡ new (mas com GC automático)
    VRAI,         // ≡ true
    FAUX,         // ≡ false
    NULLE,        // ≡ nullptr (observação: "nulle" em francês é feminino)

    // ==============================================
    // IDENTIFICADORES E LITERAIS
    // ==============================================
    IDENTIFIANT,  // ≡ identificadores C++
    NOMBRE,       // ≡ int/long (números inteiros)
    DECIMAL,      // ≡ float/double
    CHAINE,       // ≡ std::string

    // ==============================================
    // OPERADORES (mesmos símbolos, nomes em francês)
    // ==============================================
    PLUS,         // ≡ +
    MOINS,        // ≡ -
    FOIS,         // ≡ *
    DIVISE,        // ≡ /
    PERCENT,      // ≡ %
    
    // Operadores de comparação
    EGAL,         // ≡ =
    EGAL_EGAL,    // ≡ ==
    DIFFERENT,    // ≡ !=
    INFERIEUR,    // ≡ <
    INF_EGAL,     // ≡ <=
    SUPERIEUR,    // ≡ >
    SUP_EGAL,    // ≡ >=
    
    // Operadores lógicos
    ET,          // ≡ &&
    OU,           // ≡ ||
    NON,         // = !
    // ==============================================
    // DELIMITADORES (mesmo comportamento que C++)
    // ==============================================
    LEFT_PAREN,      // ≡ (
    RIGHT_PAREN,     // ≡ )
    LEFT_BRACE,      // ≡ {
    RIGHT_BRACE,     // ≡ }
    CHOCHET_OUVRANT, // = [
    CHOCHET_FERMANT, // = ]
    COMMA,           // ≡ ,
    DOT,             // ≡ .
    SEMICOLON,       // ≡ ;
    COLON,           // ≡ :

    // ==============================================
    // CONTROLE DO PARSER
    // ==============================================
    FIN_FICHIER   // ≡ EOF
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
};

class Lexer {
public:
    explicit Lexer(const std::string& source);
    std::vector<Token> scanTokens();
    
private:
    const std::string& source_;
    std::vector<Token> tokens_;
    size_t start_ = 0;
    size_t current_ = 0;
    size_t line_ = 1;
    
    //métodos auxiliares
    bool isAtEnd() const;
    char advance();
    void addToken(TokenType type);
    bool match(char expected);
    char peek() const;
    char peekNext() const;
    void string();
    void number();
    void identifier();
     
        //mapa de PALAVRAS-CHAVE
static const std::unordered_map<std::string, TokenType> Lexer::keywords = {

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

;

    
     












 
};

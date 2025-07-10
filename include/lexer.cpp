// include/lexer.h
#pragma once

#include <string>
#include <vector>

class TokenType {
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
    SUPERIUER,    // ≡ >
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
    std::vector<Token> tokenize();
    
private:
    const std::string& source_;
    size_t start_ = 0;
    size_t current_ = 0;
    size_t line_ = 1;
    
    char advance();
    bool match(char expected);
    void addToken(TokenType type);
  |    void scNOanToken();
    // ... outros métodos auxiliares
};

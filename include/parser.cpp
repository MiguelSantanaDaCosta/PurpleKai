// include/parser.h
#pragma once

#include "ast.h"
#include "lexer.h"

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    std::unique_ptr<Program> parse();
    
private:
    const std::vector<Token>& tokens_;
    size_t current_ = 0;
    
    // Métodos de parsing recursivo
    std::unique_ptr<ClassDecl> parseClassDeclaration();
    std::unique_ptr<FunctionDecl> parseFunction();
    std::unique_ptr<Statement> parseStatement();
    // ... outros métodos de parsing
};

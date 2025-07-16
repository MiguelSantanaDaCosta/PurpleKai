// include/interpreter.h
#pragma once

#include "ast.h"
#include "environment.h"

class Interpreter {
public:
    Interpreter();
    void interpret(const std::unique_ptr<Program>& program);
    
private:
    std::shared_ptr<Environment> globals_;
    std::shared_ptr<Environment> environment_;
    
    Value evaluate(const std::unique_ptr<Expr>& expr);
    void execute(const std::unique_ptr<Statement>& stmt);
    void executeBlock(const std::vector<std::unique_ptr<Statement>>& statements,
                     std::shared_ptr<Environment> env);
    // ... outros métodos de avaliação
};

#include "../include/interpreter.h"
#include "../include/parser.h" 
#include <iostream>
#include <stdexcept>

// =====================================================================
// 1. PONTO DE ENTRADA E CONTROLE DE BLOCOS
// =====================================================================
Interpreter::Interpreter() {
    // Inicialização da classe (pode ficar vazio por enquanto)
}

class BreakException {};
class ContinueException {};

void Interpreter::interpret(const std::vector<Stmt*>& statements) {
    try {
        for (Stmt* statement : statements) {
            execute(statement);
        }
    } catch (const std::runtime_error& error) {
        std::cerr << error.what() << std::endl; 
    }
}

Value Interpreter::evaluate(Expr* expr) {
    if (expr == nullptr) return Value();
    return expr->accept(this);
}

void Interpreter::execute(Stmt* stmt) {
    if (stmt != nullptr) stmt->accept(this);
}

void Interpreter::executeBlock(const std::vector<Stmt*>& statements, Environment* env) {
    Environment* previous = environment;
    environment = env;

    try {
        for (Stmt* statement : statements) {
            execute(statement);
        }
    } catch (...) {
        environment = previous;
        throw;
    }

    environment = previous; 
}

// =====================================================================
// 2. VISITANTES DE EXPRESSÕES (MATEMÁTICA E LÓGICA)
// =====================================================================

Value Interpreter::visitBinaryExpr(BinaryExpr* expr) {
    Value left = evaluate(expr->left.get());
    Value right = evaluate(expr->right.get());

    switch (expr->op.type) {
        // --- ARITMÉTICA ---
        case TokenType::PLUS:
    // Concatenação mista: se um dos operandos for string, converte o outro
            if (left.type == ValueType::STRING || right.type == ValueType::STRING) {
                std::string leftStr = (left.type == ValueType::STRING) ? std::get<std::string>(left.data) : stringify(left);
                std::string rightStr = (right.type == ValueType::STRING) ? std::get<std::string>(right.data) : stringify(right);
            return Value(leftStr + rightStr);
    }
    // Soma numérica
            if (left.type == ValueType::NUMBER && right.type == ValueType::NUMBER)
                return Value(std::get<int>(left.data) + std::get<int>(right.data));
            if (left.type == ValueType::FLOAT && right.type == ValueType::FLOAT)
                return Value(std::get<double>(left.data) + std::get<double>(right.data));
            throw std::runtime_error("Operandos devem ser dois números, dois floats ou pelo menos uma string.");
        
        case TokenType::MOINS:
            checkNumberOperands(expr->op, left, right);
            return Value(std::get<int>(left.data) - std::get<int>(right.data));

        case TokenType::FOIS:
            checkNumberOperands(expr->op, left, right);
            return Value(std::get<int>(left.data) * std::get<int>(right.data));

        case TokenType::DIVISE:
            checkNumberOperands(expr->op, left, right);
            if (std::get<int>(right.data) == 0) throw std::runtime_error("Divisão por zero não permitida.");
            return Value(std::get<int>(left.data) / std::get<int>(right.data));

        // --- COMPARAÇÃO ---
        case TokenType::EGAL_EGAL:
            return Value(isEqual(left, right));

        case TokenType::DIFFERENT:
            return Value(!isEqual(left, right));

        case TokenType::INFERIEUR:
            checkNumberOperands(expr->op, left, right);
            return Value(std::get<int>(left.data) < std::get<int>(right.data));

        case TokenType::INF_EGAL:
            checkNumberOperands(expr->op, left, right);
            return Value(std::get<int>(left.data) <= std::get<int>(right.data));

        case TokenType::SUPERIEUR:
            checkNumberOperands(expr->op, left, right);
            return Value(std::get<int>(left.data) > std::get<int>(right.data));

        case TokenType::SUP_EGAL:
            checkNumberOperands(expr->op, left, right);
            return Value(std::get<int>(left.data) >= std::get<int>(right.data));

        default:
            throw std::runtime_error("Operador binário não suportado.");
    }
}

Value Interpreter::visitUnaryExpr(UnaryExpr* expr) {
    Value right = evaluate(expr->right.get());

    switch (expr->op.type) {
        case TokenType::MOINS:
            checkNumberOperand(expr->op, right);
            return Value(-std::get<int>(right.data));
        case TokenType::NON:
            return Value(!isTruthy(right));
        default:
            throw std::runtime_error("Operador unário não implementado");
    }
}

Value Interpreter::visitLiteralExpr(LiteralExpr* expr) {
    return expr->value; 
}

Value Interpreter::visitGroupingExpr(GroupingExpr* expr) {
    return evaluate(expr->right.get());
}

Value Interpreter::visitAssignExpr(AssignExpr* expr) {
    Value value = evaluate(expr->valeu.get()); // Mantido 'valeu' conforme sua AST
    environment->assign(expr->name, value); 
    return value;
}

Value Interpreter::visitLogicalExpr(LogicalExpr* expr) {
    Value left = evaluate(expr->left.get());
    
    if (expr->op.type == TokenType::OU) {
        if (isTruthy(left)) return left;
    } else { // TokenType::ET
        if (!isTruthy(left)) return left;
    }
    
    return evaluate(expr->right.get());
}

Value Interpreter::visitVariableExpr(VariableExpr* expr) { 
    return environment->get(expr->value); // Corrigido de 'name' para 'identifier'
}

// Mocks temporários para features avançadas
Value Interpreter::visitCallExpr(CallExpr* expr) { return Value(); } 
Value Interpreter::visitGetExpr(GetExpr* expr) { return Value(); }
Value Interpreter::visitSetExpr(SetExpr* expr) { return Value(); }
Value Interpreter::visitThisExpr(ThisExpr* expr) { return Value(); }
Value Interpreter::visitSuperExpr(SuperExpr* expr) { return Value(); }


// =====================================================================
// 3. VISITANTES DE COMANDOS (STATEMENTS)
// =====================================================================

void Interpreter::visitExprStmt(ExprStmt* stmt) {
    Value value = evaluate(stmt->expression.get());
    
    // Comportamento do REPL: Se estivermos avaliando uma expressão solta no terminal,
    // queremos ver o resultado impresso diretamente!
    if (environment != nullptr) { 
        std::cout << stringify(value) << std::endl;
    }
}

void Interpreter::visitBreakStmt(BreakStmt* stmt) {
    if (!inLoop) throw std::runtime_error("Break outside loop");
    throw BreakException();   // precisamos de uma exceção especial
}

void Interpreter::visitContinueStmt(ContinueStmt* stmt) {
    if (!inLoop) throw std::runtime_error("Continue outside loop");
    throw ContinueException();
}

void Interpreter::visitVarDec1(VarDec1* stmt) {
    Value value;
    if (stmt->initializer != nullptr) {
        value = evaluate(stmt->initializer.get());
    }
    environment->define(stmt->name.lexeme, value);
}

void Interpreter::visitBlockStmt(BlockStmt* stmt) {
    Environment blockEnv(environment);
    
    // Extrai ponteiros brutos dos unique_ptrs para compatibilidade com executeBlock
    std::vector<Stmt*> rawStatements;
    rawStatements.reserve(stmt->statements.size());
    for (const auto& statement : stmt->statements) {
        rawStatements.push_back(statement.get());
    }

    executeBlock(rawStatements, &blockEnv);
}

void Interpreter::visitIfStmt(IfStmt* stmt) {
    if (isTruthy(evaluate(stmt->condition.get()))) {
        execute(stmt->thenBranch.get());
    } else if (stmt->elseBranch != nullptr) {
        execute(stmt->elseBranch.get());
    }
}

void Interpreter::visitWhileStmt(WhileStmt* stmt) {
    bool wasInLoop = inLoop;
    inLoop = true;

    try {
        while (isTruthy(evaluate(stmt->condition.get()))) {
            try {
                execute(stmt->body.get());
            } catch (BreakException&) {
                break;
            } catch (ContinueException&) {
                continue;
            }
        }
    } catch (...) {
        inLoop = wasInLoop;
        throw;
    }

    inLoop = wasInLoop;
}


void Interpreter::visitForStmt(ForStmt* stmt) {
    Environment* previous = environment;
    Environment forEnv(environment);
    environment = &forEnv;

    try {
        if (stmt->initializer != nullptr) {
            execute(stmt->initializer.get());
        }

        bool wasInLoop = inLoop;
        inLoop = true;

        while (stmt->condition == nullptr || isTruthy(evaluate(stmt->condition.get()))) {
            try {
                execute(stmt->body.get());
            } catch (BreakException&) {
                break;
            } catch (ContinueException&) {
                // Continue: apenas interrompe o corpo, o incremento será executado normalmente
                // (não precisa fazer nada aqui, apenas sair do catch)
            }
            // Executa o incremento após o corpo (tanto se for normal quanto se for continue)
            if (stmt->increment != nullptr) {
                evaluate(stmt->increment.get());
            }
        }

        inLoop = wasInLoop;
    } catch (...) {
        environment = previous;
        throw;
    }
    environment = previous;
}

// Mocks obrigatórios para compilar
void Interpreter::visitReturnStmt(ReturnStmt* stmt) {}
void Interpreter::visitFunctionDec1(FunctionDec1* stmt) {}
void Interpreter::visitClassDec1(ClassDec1* stmt) {}


// =====================================================================
// 4. FUNÇÕES UTILITÁRIAS
// =====================================================================

bool Interpreter::isTruthy(const Value& value) {
    if (value.type == ValueType::NIL) return false;
    if (value.type == ValueType::BOOL) return std::get<bool>(value.data);
    return true;
}

bool Interpreter::isEqual(const Value& a, const Value& b) {
    if (a.type != b.type) return false;
    
    switch (a.type) {
        case ValueType::NIL: return true;
        case ValueType::BOOL: return std::get<bool>(a.data) == std::get<bool>(b.data);
        case ValueType::NUMBER: return std::get<int>(a.data) == std::get<int>(b.data);
        case ValueType::STRING: return std::get<std::string>(a.data) == std::get<std::string>(b.data);
        default: return false; 
    }
}

void Interpreter::checkNumberOperand(const Token& op, const Value& operand) {
    if (operand.type == ValueType::NUMBER) return;
    throw std::runtime_error("Operando deve ser um número");
}

void Interpreter::checkNumberOperands(const Token& op, const Value& left, const Value& right) {
    if (left.type == ValueType::NUMBER && right.type == ValueType::NUMBER) return;
    throw std::runtime_error("Operandos devem ser números");
}

std::string Interpreter::stringify(const Value& value) {
    switch (value.type) {
        case ValueType::NIL: return "nul";
        case ValueType::BOOL: return std::get<bool>(value.data) ? "vrai" : "faux";
        case ValueType::NUMBER: return std::to_string(std::get<int>(value.data));
        case ValueType::STRING: return std::get<std::string>(value.data);
        default: return "[object]";
    }
}

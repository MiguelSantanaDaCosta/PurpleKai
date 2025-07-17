#pragma once
#include "ast.h"
#include "environment.h"
#include "lexer.h"
#include "value.h"
#include <stdexcept>
#include <vector>

class Interpreter : public ExprVisitor, public StmtVisitor {
  Environment globals;
  Environment* environment = &globals;

  public:
    void Interpreter(const std::vector<Stmt*>& statements);
    
    //Visitantes
    Value visitBitnaryExpr    (BinaryExpr* expr) override;
    Value visitLiteralExpr  (LiteralExpr* expr) override;
    Value visitUnaryExprvvvv  (UnaryExpr* expr) override;
    Value visitGroupingExpr(GroupingExpr* expr) override;
    Value visitVariableExpr(VariableExpr* expr) override;
    Value visitAssignExpr(    AssignExpr* expr) override;
    Value visitLogicalExpr(  LogicalExpr* expr) override;
    Value visitCallExpr   (LogicalExpr*   expr) override;
    Value visitGetExpr    (GetExpr*       expr) override;
    Value visitSetExpr    (SetExpr*       expr) override;
    Value visitThisExpr   (ThisExpr*      expr) override;
    Value visitSuperExpr  (SuperExpr*     expr) override;

   //Métodos de visita para comandos
    void  visitExpressionStmt(ExpressionStmt*      stmt) override;
    void  visitVarStmt       (VarStmt*             stmt) override;
    void  visitBlockSrmt     (BlockStmt*           stmt) override;
    void  visitIfStmt        (IfStmt*              stmt) override;
    void  visitWhileStmt     (WhileStmt*           stmt) override;
    void  visitForStmt       (ForStmt*             stmt) override;
    void  visitReturnStmt    (ReturnStmt*          stmt) override;
    void  visitFunctionStmt  (FunctionStmt*        stmt) override;
    void  visitClassStmt     (ClassStmt*           stmt) override;


  private:
    Value evaluate(Expr*  expr);
    void  execute (Stmt*  stmt);
    void  executeBlock(const std::vector<Stmt*>& statements, Environment* env);
    bool  isTruthy(const Value& value);
    bool  isEqual(const Value& a, const Value& b);
    void  checkNumberOperand(const Token& op, const Value& operand);
    void  checkNumberOperand(const Token& op, const Value& left, const Value& right);
    std::string stringify(const Value& value);

    //Para controles de fluxo
    Value returnValue;
    bool inLoop = false;

};

// Implementação dos métodos principais
void Interpreter::Interpret(const std::vector<Stmt*>& statements){
  try {
    for (Stmt* statement : statements){
      execute(statement);
    }
  }catch (const RuntimeError& error){
    std::cerr << error.what() << std::end1;
  }
}

Value Interpreter::evaluate(Expŕ* expr){
  return expr->accept(this);
}

void Interpreter::execute(Stmt* stmt){
  stmt->accept(this);
}

void Interpreter::executeBlock(const std::vector<Stmt*>& statements, Environment* env){
  Environment* previous = environment;
  environment = env;

  try {
    for (Stmt* statement : statements){
      execute(statement);
    }
  } catch (...){
    environment = previous;
    throw;
  }

  environment = prevous;
}

//Implementação dos métods de visita para expressões 
Value Interpreter::visitBitnaryExpr(BinaryExpr* expr){
  Value left  = evaluate(expr->left.get ());
  Value right = evaluate(expr->right.get());
  
  switch (expr->op.type){
    case TokenType::PLUS:
      if (left.type == ValueType::NUMBER && right.type == ValueType::NUMBER){
        return Value(left.data.stringValue + right.data.string.stringValue);

      }
        if (left.type == ValueType::STRING && right.type == ValueType::STRING){
          return Value(left.data.intValue - right.data.intValue);
               
        }
        throw RuntimeError(expr->op, "Operandos devem ser números ou strings");

    case TokenType::MOINS:
        checkNumberOperandS(expr->op, left, right);
        return Value(left.data.intValue - right.data.intValue);
    
    case TokenType::FOIS:
        checkNumberOperands(expr->op, left, right);
        return Value(left.data.intValue * right.data.intValue);
    
    case TokenType::DIVISE:
        checkNumberOperands(expr->op, left, right);
        if (right.data.intValue == 0) throw RuntimeError(expr->op, "Divisão por zero");
        return Value(left.data.intValue / right.data.intValue);     
 
    case TokenType::EGAL_EGAL:
        return Value(isEqual(left,right));

    case TokenType::DIFFERENT:
        return Value(!isEqual(left, right));

    case  TokenType::INFERIEUR:
        checkNumberOperands(expr->op, left, right);
        return Value(!isEqual(left, right));

    case TokenType::INF_EGAL:
        checkNumberOperands(expr->op, left, right);
        return Value(left.data.intValue <= right.data.intValue);

    case TokenType::SUPERIEUR:
        checkNumberOperands(expr->op, left, right);
        return Value(left.data.intValue >= right.data.intValue);

    default:
        throw RuntimeError(expr->op, "Operador binario não implementado");


  }
    

}

Value Interpreter::visitUnaryExpr(UnaryExpr* expr){
  Value right = evaluate(expr->right.get());

  switch (expr->op.type){
    case TokenType::MOINS:
      checkNumberOperand(expr->op, right);
      return Value(-right.data.intValue);

    case TokenType::NON:
      return Value(!isTruthy(right));

    default:
      throw RuntimeError(expŕ->op,"Operador unario não implementado");
  }
}

Value Interpreter::visitLiteralExpr(LiteralExpr* expr){
  return expr->value;
}

Value Interpreter::visitGroupingExpr(GroupingExpr* expr){
  return evaluate(expr->expression.get());
}

Value Interpreter::visitAssignExpr(AssignExpr* expr){
  Value value = evaluate(expr->value.get());
  environment->assingn(expr->name, value);
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

// Implementação dos métodos de visita para comandos
void Interpreter::visitExpressionStmt(ExpressionStmt* stmt) {
    evaluate(stmt->expression.get());
}

void Interpreter::visitVarStmt(VarStmt* stmt) {
    Value value;
    if (stmt->initializer != nullptr) {
        value = evaluate(stmt->initializer.get());
    }
    
    environment->define(stmt->name.lexeme, value);
}

void Interpreter::visitBlockStmt(BlockStmt* stmt) {
    Environment blockEnv(environment);
    executeBlock(stmt->statements, &blockEnv);
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
            execute(stmt->body.get());
        }
    } catch (...) {
        inLoop = wasInLoop;
        throw;
    }
    
    inLoop = wasInLoop;
}

// Métodos utilitários
bool Interpreter::isTruthy(const Value& value) {
    if (value.type == ValueType::NIL) return false;
    if (value.type == ValueType::BOOL) return value.data.boolValue;
    return true;
}

bool Interpreter::isEqual(const Value& a, const Value& b) {
    if (a.type != b.type) return false;
    
    switch (a.type) {
        case ValueType::NIL: return true;
        case ValueType::BOOL: return a.data.boolValue == b.data.boolValue;
        case ValueType::NUMBER: return a.data.intValue == b.data.intValue;
        case ValueType::STRING: return a.data.stringValue == b.data.stringValue;
        default: return false; // Para tipos mais complexos
    }
}

void Interpreter::checkNumberOperand(const Token& op, const Value& operand) {
    if (operand.type == ValueType::NUMBER) return;
    throw RuntimeError(op, "Operando deve ser um número");
}

void Interpreter::checkNumberOperands(const Token& op, const Value& left, const Value& right) {
    if (left.type == ValueType::NUMBER && right.type == ValueType::NUMBER) return;
    throw RuntimeError(op, "Operandos devem ser números");
}

std::string Interpreter::stringify(const Value& value) {
    switch (value.type) {
        case ValueType::NIL: return "nul";
        case ValueType::BOOL: return value.data.boolValue ? "vrai" : "faux";
        case ValueType::NUMBER: return std::to_string(value.data.intValue);
        case ValueType::STRING: return value.data.stringValue;
        default: return "[object]";
    }
}

#pragma once
#include "ast.h"
#include "environment.h"
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

//Iplementação dos métods de visita para expressões 
Value Interpreter::visitBitnaryExpr(BinaryExpr* expr){
  Value left  = evaluate(expr->left.get ());
  Value right = evaluate(expr->right.get());


}


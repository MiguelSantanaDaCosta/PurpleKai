#pragma once

#include "parser.h"
#include "environment.h"
#include "lexer.h"
#include "value.h"
#include <stdexcept>
#include <vector>
#include <string>

class Interpreter : public ExprVisitor, public StmtVisitor {
public:
    Interpreter();
    void interpret(const std::vector<Stmt*>& statements);

    // Visitantes de Expressões
    Value visitBinaryExpr(BinaryExpr* expr) override;
    Value visitLiteralExpr(LiteralExpr* expr) override;
    Value visitUnaryExpr(UnaryExpr* expr) override;
    Value visitGroupingExpr(GroupingExpr* expr) override;
    Value visitVariableExpr(VariableExpr* expr) override;
    Value visitAssignExpr(AssignExpr* expr) override;
    Value visitLogicalExpr(LogicalExpr* expr) override;
    Value visitCallExpr(CallExpr* expr) override;
    Value visitGetExpr(GetExpr* expr) override;
    Value visitSetExpr(SetExpr* expr) override;
    Value visitThisExpr(ThisExpr* expr) override;
    Value visitSuperExpr(SuperExpr* expr) override;

    // Visitantes de Comandos
    void visitExprStmt(ExprStmt* stmt) override;
    void visitVarDec1(VarDec1* stmt) override;
    void visitBlockStmt(BlockStmt* stmt) override;
    void visitIfStmt(IfStmt* stmt) override;
    void visitWhileStmt(WhileStmt* stmt) override;
    void visitForStmt(ForStmt* stmt) override;
    void visitReturnStmt(ReturnStmt* stmt) override;
    void visitFunctionDec1(FunctionDec1* stmt) override;
    void visitClassDec1(ClassDec1* stmt) override;
    void visitBreakStmt(BreakStmt* stmt) override;
    void visitContinueStmt(ContinueStmt* stmt) override;

private:
    Environment globals;
    Environment* environment = &globals;

    Value evaluate(Expr* expr);
    void execute(Stmt* stmt);
    void executeBlock(const std::vector<Stmt*>& statements, Environment* env);

    bool isTruthy(const Value& value);
    bool isEqual(const Value& a, const Value& b);
    void checkNumberOperand(const Token& op, const Value& operand);
    void checkNumberOperands(const Token& op, const Value& left, const Value& right);
    std::string stringify(const Value& value);

    Value returnValue;
    bool inLoop = false;
};

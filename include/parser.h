#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include <functional>
#include <unordered_map>
#include "lexer.h"
#include "type.h"
#include "value.h"

// ============================================================
// FORWARD DECLARATIONS
// ============================================================
class Program;
class ClassDec1;
class FunctionDec1;
class VarDec1;
class BlockStmt;
class ExprStmt;
class IfStmt;
class ForStmt;
class ReturnStmt;
class WhileStmt;
class Expr;
class AssignExpr;
class BinaryExpr;
class LogicalExpr;
class CallExpr;
class VariableExpr;
class LiteralExpr;
class GroupingExpr;
class UnaryExpr;
class GetExpr;
class SetExpr;
class ThisExpr;
class SuperExpr;
class BreakStmt;
class ContinueStmt;

// ============================================================
// BASE CLASSES
// ============================================================
class Declaration {
public:
    virtual ~Declaration() = default;
};

class Expr {
public:
    virtual ~Expr() = default;
    virtual Value accept(class ExprVisitor* visitor) = 0;
};

class Stmt : public Declaration {
public:
    virtual ~Stmt() = default;
    virtual void accept(class StmtVisitor* visitor) = 0;
};

// ============================================================
// ALIASES (depois das classes base)
// ============================================================
using ExprPtr = std::unique_ptr<Expr>;
using StmtPtr = std::unique_ptr<Stmt>;
using DeclPtr = std::unique_ptr<Declaration>;

// ============================================================
// VISITOR INTERFACES
// ============================================================
class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    virtual Value visitBinaryExpr(BinaryExpr* expr) = 0;
    virtual Value visitLiteralExpr(LiteralExpr* expr) = 0;
    virtual Value visitUnaryExpr(UnaryExpr* expr) = 0;
    virtual Value visitGroupingExpr(GroupingExpr* expr) = 0;
    virtual Value visitVariableExpr(VariableExpr* expr) = 0;
    virtual Value visitAssignExpr(AssignExpr* expr) = 0;
    virtual Value visitLogicalExpr(LogicalExpr* expr) = 0;
    virtual Value visitCallExpr(CallExpr* expr) = 0;
    virtual Value visitGetExpr(GetExpr* expr) = 0;
    virtual Value visitSetExpr(SetExpr* expr) = 0;
    virtual Value visitThisExpr(ThisExpr* expr) = 0;
    virtual Value visitSuperExpr(SuperExpr* expr) = 0;
};

class StmtVisitor {
public:
    virtual ~StmtVisitor() = default;
    virtual void visitExprStmt(ExprStmt* stmt) = 0;
    virtual void visitVarDec1(VarDec1* stmt) = 0;
    virtual void visitBlockStmt(BlockStmt* stmt) = 0;
    virtual void visitIfStmt(IfStmt* stmt) = 0;
    virtual void visitWhileStmt(WhileStmt* stmt) = 0;
    virtual void visitForStmt(ForStmt* stmt) = 0;
    virtual void visitReturnStmt(ReturnStmt* stmt) = 0;
    virtual void visitFunctionDec1(FunctionDec1* stmt) = 0;
    virtual void visitClassDec1(ClassDec1* stmt) = 0;
    virtual void visitBreakStmt(BreakStmt* stmt) = 0;
    virtual void visitContinueStmt(ContinueStmt* stmt) = 0;
};

class ParseError : public std::runtime_error {
public:
    ParseError(const Token& token, const std::string& message)
        : std::runtime_error(message), token(token) {}
    const Token token;
};

// ============================================================
// PARSER CLASS
// ============================================================
class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    std::unique_ptr<Program> parse();

private:
    const std::vector<Token>& tokens_;
    size_t current_ = 0;
    bool hadError = false;

    bool isAtEnd() const;
    const Token& peek() const;
    const Token& previous() const;
    const Token& advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    bool match(const std::initializer_list<TokenType>& types);
    const Token& consume(TokenType type, const std::string& message);
    const Token& expect(TokenType type, const std::string& message) const;

    ParseError error(const Token& token, const std::string& message) const;
    void reportError(const Token& token, const std::string& message);
    void synchronize();

    StmtPtr parseDeclaration();
    std::unique_ptr<ClassDec1> parseClasseDeclaration();
    std::unique_ptr<FunctionDec1> parseFunctionDeclaration();
    std::unique_ptr<VarDec1> parseVarDeclaration();
    std::unique_ptr<Type> parseType();

    StmtPtr parseStatement();
    StmtPtr parseExprStmt();
    std::unique_ptr<BlockStmt> parseBlock();
    StmtPtr parseIfStatement();
    StmtPtr parseForStatement();
    StmtPtr parseWhileStatement();
    StmtPtr parseReturnStatement();

    ExprPtr parseExpression();
    ExprPtr parseAssigment();
    ExprPtr parseOr();
    ExprPtr parseAnd();
    ExprPtr parseEquality();
    ExprPtr parseComparison();
    ExprPtr parseTerm();
    ExprPtr parseFactor();
    ExprPtr parseUnary();
    ExprPtr parseCall();
    ExprPtr parsePrimary();
    ExprPtr finishCall(ExprPtr callee);
    std::vector<ExprPtr> parseArguments();
    std::vector<Token> parseParameters();
};

// ============================================================
// AST NODES – DECLARATIONS / STATEMENTS
// ============================================================
class Program : public Declaration {
public:
    std::vector<DeclPtr> declarations;
};

class ClassDec1 : public Stmt {
public:
    Token name;
    std::unique_ptr<VariableExpr> superclass;
    std::vector<std::unique_ptr<FunctionDec1>> methods;

    void accept(StmtVisitor* visitor) override { visitor->visitClassDec1(this); }
};

class FunctionDec1 : public Stmt {
public:
    FunctionDec1() = default;
    FunctionDec1(Token name, std::vector<Token> params,
                 std::unique_ptr<Type> returnType,
                 std::unique_ptr<BlockStmt> body)
        : name(name), params(std::move(params)),
          returnType(std::move(returnType)), body(std::move(body)) {}

    Token name;
    std::vector<Token> params;
    std::unique_ptr<Type> returnType;
    std::unique_ptr<BlockStmt> body;

    void accept(StmtVisitor* visitor) override { visitor->visitFunctionDec1(this); }
};

class VarDec1 : public Stmt {
public:
    Token name;
    ExprPtr initializer;
    bool isConst;

    void accept(StmtVisitor* visitor) override { visitor->visitVarDec1(this); }
};

class BlockStmt : public Stmt {
public:
    std::vector<StmtPtr> statements;

    void accept(StmtVisitor* visitor) override { visitor->visitBlockStmt(this); }
};

class ExprStmt : public Stmt {
public:
    ExprPtr expression;

    void accept(StmtVisitor* visitor) override { visitor->visitExprStmt(this); }
};

class IfStmt : public Stmt {
public:
    ExprPtr condition;
    StmtPtr thenBranch;
    StmtPtr elseBranch;

    void accept(StmtVisitor* visitor) override { visitor->visitIfStmt(this); }
};

class ForStmt : public Stmt {
public:
    StmtPtr initializer;
    ExprPtr condition;
    ExprPtr increment;
    StmtPtr body;

    void accept(StmtVisitor* visitor) override { visitor->visitForStmt(this); }
};

class WhileStmt : public Stmt {
public:
    ExprPtr condition;
    StmtPtr body;

    WhileStmt(ExprPtr condition, StmtPtr body)
        : condition(std::move(condition)), body(std::move(body)) {}

    void accept(StmtVisitor* visitor) override { visitor->visitWhileStmt(this); }
};

class ReturnStmt : public Stmt {
public:
    Token keyword;
    ExprPtr value;

    ReturnStmt(const Token& keyword, ExprPtr value)
        : keyword(keyword), value(std::move(value)) {}

    void accept(StmtVisitor* visitor) override { visitor->visitReturnStmt(this); }
};

// ============================================================
// AST NODES – EXPRESSIONS
// ============================================================
class AssignExpr : public Expr {
public:
    Token name;
    ExprPtr valeu;

    Value accept(ExprVisitor* visitor) override { return visitor->visitAssignExpr(this); }
};

class BinaryExpr : public Expr {
public:
    ExprPtr left;
    Token op;
    ExprPtr right;

    Value accept(ExprVisitor* visitor) override { return visitor->visitBinaryExpr(this); }
};

class LogicalExpr : public Expr {
public:
    ExprPtr left;
    Token op;
    ExprPtr right;

    Value accept(ExprVisitor* visitor) override { return visitor->visitLogicalExpr(this); }
};

class CallExpr : public Expr {
public:
    ExprPtr callee;
    Token paren;
    std::vector<ExprPtr> arguments;

    Value accept(ExprVisitor* visitor) override { return visitor->visitCallExpr(this); }
};

class VariableExpr : public Expr {
public:
    Token value;   // nome da variável

    Value accept(ExprVisitor* visitor) override { return visitor->visitVariableExpr(this); }
};

class LiteralExpr : public Expr {
public:
    Value value;

    Value accept(ExprVisitor* visitor) override { return visitor->visitLiteralExpr(this); }
};

class GroupingExpr : public Expr {
public:
    Token op;
    ExprPtr right;

    Value accept(ExprVisitor* visitor) override { return visitor->visitGroupingExpr(this); }
};

class UnaryExpr : public Expr {
public:
    Token op;
    ExprPtr right;

    Value accept(ExprVisitor* visitor) override { return visitor->visitUnaryExpr(this); }
};

class GetExpr : public Expr {
public:
    ExprPtr object;
    Token name;

    Value accept(ExprVisitor* visitor) override { return visitor->visitGetExpr(this); }
};

class SetExpr : public Expr {
public:
    ExprPtr object;
    Token name;
    ExprPtr valeu;

    Value accept(ExprVisitor* visitor) override { return visitor->visitSetExpr(this); }
};

class ThisExpr : public Expr {
public:
    Token keyword;

    Value accept(ExprVisitor* visitor) override { return visitor->visitThisExpr(this); }
};

class SuperExpr : public Expr {
public:
    Token keyword;
    Token method;

    Value accept(ExprVisitor* visitor) override { return visitor->visitSuperExpr(this); }
};

class BreakStmt : public Stmt {
public:
    void accept(StmtVisitor* visitor) override { visitor->visitBreakStmt(this); }
};

class ContinueStmt : public Stmt {
public:
    void accept(StmtVisitor* visitor) override { visitor->visitContinueStmt(this); }
};

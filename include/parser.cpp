#include <vector>
#include <memory>
#include <stdexcept>
#include <functional>
#include <unordered_map>
#include "lexer.h"

//===============================================
//FORWARD DECLARATIONS PARA  OS NÓS DA AST
//================================================

class Program;
class ClassDec1;
class FunctionDec1;
class VarDec1;
class BlockStmt;
class Expr;
class AssignExpr;
class BinaryExpr;
class LogicalExpr;
class CallExpr;
class VariableExpr;
class LiteralExpr;
class GroupingExpr;
class UnaryExpt;
class GetExpr;
class SetExpr;
class ThisExpr;
class SuperExpr;

//=============================
//DEFINIÇÕES DE TIPOS 
//=============================
using ExprPtr = std::unique_ptr<Expr>;
using StmtPtr = std::unique_ptr< class Stmt>;
using Dec1Ptr = std::unique_ptr<class Declaration>;

//=========================
//CLASSE PARA DECLARAÇÕES
//=========================
class Declaration {
public:
   virtual ~Declaration() = default;
};

//====================================
//CLASSE PARA COMANDOS (STATEMENTS)
//====================================
class Stmt {
  public:
    virtual ~Stmt() = default;
};

//===================================
//CLASSE PARA EXPRESSÕES
//===================================
class Expr {
  public:
    virtual ~Expr() = default;
};
















































































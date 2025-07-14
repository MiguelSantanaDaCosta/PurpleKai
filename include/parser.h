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

//===================================
//DEFINIÇÕES DA CLASSE PARSER
//===================================
class Parser {
  public:
    /**Construtor: Recebe os tokens gerados pelo lexer
     * @param tokens Lista de tokens para análise */
    explicit Parser(const std::vector<Token>& tokens);
    /**Metodo principal: Imicia o processo de parsing
     * @return AST cmopleta do programa */ 
    std::unique_ptr<Program> parse();
}

   private:
  //ESTADO INTERMO
  const std::vector<Token>& tokens_;//referencia aos tokens
  size_t current_ = 0;              //Indice do token atual

  //METODO AUXILIARES DE CONTROLE
  
  //verifica se chegou ao fim dos tokens
  bool isAtEnd() const;

  //Avança para o proximo token e retorna o anterior
  const Token& peek() const;

  //retorna o token anteriormente consumido
  const Token& previous() const;

  /**Verifica se o token atuak corresponde ao tipo esperado
  @param type Tipo de token a verificar
  @return true se corresponde, false caso contrario
  */ 
  bool check(TokenType type) const;
  /**Verifica e consome se o token corresponder ao tipo
  @paaram type Tipo de token esperado
  @return true se corresponde e foi consumido
  */ //    
  
  bool match(TokenType type);
  /** Verifica e consome se o token corresponder ao tipo
   * @param type Tipo de token esperado
   * @return true se corresponde e foi consumido
   */
  bool match(const std::initializer_list<TokenType>& types);
  
  /**
   * Consome obrigatoriamente um token do tipo especificado
   * @param type Tipo esperado do token
   * @param message Mensagem de erro se não corresponder
   * @return Token consumido
   * @throws ParseError se o tipo não corresponder
   */
  const Token& consume(TokenType type, const std::string& message);
  /**
   * Verifica se o token atual é de um tipo especifico sem comsumir
   * @param type Tipo a verificar
   * @param message Mensagem de erro se não corresponder
   * @return Token verificado
   */

  const Token& expect(TokenType type, const std::string& message) const;

  //===================================================================
  //MANIPULAÇÃO DE ERROS
  //===================================================================

  /**
   * cria um erro de parsing
   * @param token Token onde ocorreu o erro
   * @param message Descrição do erro
   * @return Exceção de erro
   *///

  ParseError error(const Token& token, const std::string& message) const;

  /**
   * Sincronoza após um erro - avança até um ponto seguro
   */
  void synchronize();

  //===================================================================
  //REGRAS DE DECLARAÇÕES (TOP-LEVEL)
  //===================================================================
  /**Parseia uma declaração ( classe, funçaõ, variavel, etc.)*///bin/X

  DeclPtr parseDeclaration();

  //Parseia declaração de classe 
  //formato: "classe" IDENTIFIER ["hérite" IDENTIFIER] "{" { member} "}"

  std::unique_ptr<ClassDec1> parseClasseDeclaration();
  /** Parsei declaração de função 
   *  Formato: ("laisser") | "constant") IDENTIFIER ["="] expression ] ";"
   * /bin/X
   *
   */
  std:: unique_ptr<VarDec1> parseVarDeclaration();

  //=====================================================================
  //REGRAS DE COMANDOS(STATEMENTS)
  //=====================================================================
  /** Parseia um comando
   * 
   */
  StmtPtr parseStatement();
  //parseia comando de expressão (expressão seguida de";")
  StmtPtr parseExprStmt();
  //parseia bloco de código: "{"{declaration } "}"
  std::unique_ptr<BlockStmt> parseBlock();
  //Parseia comando condicional (if)
  //Formato :"si" "(" expression")" statement ["sinon" statement]
  

  StmtPtr parseIfStatement();
  /**Parseia loop for
   *
   * Formato: "pour" "(" (varDec1 | exprStmt | ";")
   *             expression? ";"
   *             expression? ")" statement
   *
   */
  StmtPtr parseForStatement();
  /**
   * Parsia loop while
   * Formato: "tantque""(" expression ")" statement
   */
  StmtPtr parseWhileStatement();
  /**Parseia comando de retorno
   * Formato: "retourner" [espression] ";"
   */
  StmtPtr parseReturnStatement();

//========================================================================
//REGRAS DE EXPRESSÕES ( COM PRECEDENCIA)
//========================================================================
/** Ponto de entrada para expressões 
 */
  ExprPtr parseExpression();
  //Parseia atribuição ( menor precedência)
  //
  //
  ExprPtr parseAssigment();

  //Parseia operador lógico OR(||)

  ExprPtr parseOr();
 /** Parseia operador lógico AND(&&)
  */
  ExprPtr parseAnd();
  /** Parseia operador de igualdade(==, !=)
   */
  ExprPtr parseEquality();
  /** Parseia operadores de comparação (<,>,<=,>=)
    */  
  ExprPtr parseComparison();
  /** Parseia operadores de termo (+,-)
   */

  ExprPtr parseTerm();
  /** Parseia operadores de factor(*,/)  
  */
 ExprPtr parseFactor();
 /** Parseia operadores unários (!, -)
  */
 ExprPtr parseUnary();
 /** Parseia chamadas, acesso a membros, etc.
  */
ExprPtr parseCall();
/** Parseia expressões primarias (literais, variaveis, agrupamentos)
 */
ExprPtr parsePrimary();
/**
 * Parseia lista de argumentos para chamadas
 * @return Vector de expressões argumento
 */
std::vector<ExprPtr> parseArgumenyts();
/**
 * Parseia lista de parâmetros para funções 
 * @return Vector de tokens identificador
 */ 
std::vector<Token> parseParameters();
};

//================================================================
//DEFINIÇÕES DETALHADAS DOS NÓS DA AST
//================================================================
class Program : public Declaration {
 public:
   std::vector<Dec1Ptr> declaration;

};

class ClassDec1 : public Declaration {
 public:
   Token name;
   std::unique_ptr<VariableExpr> superclass; //Pode ser nulo
   std::vector<std::unique_ptr<FunctionDec1>> methods;
};

class FunctionDec1 : public Declaration {
  public:
    Token name;
    std::vector<Token> params;
    std:: unique_ptr<BlockStmt> body;
};

class VarDec1 : public Declaration {
  public:
    Token name;
    ExprPtr initializer;
    bool isConst;
};

class BlockStmt : public Stmt {
  public:
    ExprPtr condition;
    StmtPtr thenBranch;
    StmtPtr elseBranch;// pode ser nulo
};

class ExprStmt : public Stmt {
  public:
    ExprPtr expression;
};

class IfStmt : public Stmt {
  public:
    ExprPtr condition;
    Stmt thenBranch;
    StmtPtr elseBranch;//pode ser nulo
};

class ForStmt : public Stmt {
  public:
    StmtPtr initializer; //Pode ser nulo (VarDec1 ou ExprStmt)
    ExprPtr condition;   //Podes ser nulo
    ExprPtr increment;   //Pode ser nulo
    StmtPtr body;
}

class WhileStmt : public Stmt {
  public:
    ExprPtr condition;
    StmtPtr body;
};

class ReturnStmt : public Stmt {
  public:
    Token keyword;
    ExprPtr value;//pode ser nulo
};

//=============================================================
//EXPRESSÕES 
//=============================================================
class AssignExpr : public Expr {
  public:
    Token name;
    ExprPtr valeu;

};

class BinaryExpr : public Expr {
  public:
    ExprPtr left;
    Token op;
    ExprPtr right;
};

class LogicalExpr : public Expr {
  public:
    ExprPtr left;
    Token op;
    ExprPtr right;
};

class CallExpr : public Expr {
  public: 
    ExprPtr callee;
    Token paren;// Token do parêntese de fechamento
    std::vector<ExprPtr> arguments;
};

class VariableExpr : public Expr {
  public:
    Token value;
};

class LiteralExpr : public Expr {
  public:
    ExprPtr expression;
};

class GroupingExpr : public Expr {
  public:
    Token op;
    ExprPtr right;
};

class GetExpr : public Expr {
  public:
    ExprPtr objecct;
    Token name;
};

class SetExpr : public Expr {
  public:
    ExprPtr object;
    Token name;
    ExprPtr valeu;
};

class ThisExpr : public Expr {
  public:
    Token keyword;
};

class SuperExpr : public Expr {
  public:
    Token keyword;
    Token method;
};

//============================================
//EXCEÇÕES 
//===========================================
class Parseerror : public std::runtime_error{
  public:
    Parseerror(const Token& token, const std::string& message)
      : std::runtime_error(message), token(token){}

    const Token token;
};

































































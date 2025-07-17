#include "lexer.cpp"
#include "parser.h"
#include "lexer.h"
#include <cctype>
#include <algorithm>
#include <initializer_list>
#include <iostream>

//=================================================
//IMPLEMENTAÇÃO DOS METODOS AUXILIARES
//=================================================
Parser::Parser(const std::vector<Token& tokens)
  : tokens_(tokens){}


  /** Verifica se chegamos ao final da lista de tokens
   * @return true se o token atual é END_OF_FILE
   */
  bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
  }

/** Avança para o proximo token e retorna o token anterior
 * @return Token consumido
 */ 

const Token& Parser::advance(){
  if(!asAtEnd()) current_++;
  return previous();
}

/** Obtém o token atual sem consumi-lo (loookhead)
 * @return Token atual
 */
const Token& Parser::peek() const {
  return tokens_[current_ -1];
}

/** Verifica e consume o token se for do tipo especificado
 * @param type Tipo de token esperado
 * @return true se o token foi consumido
 */

bool Parser::match(TokenType type){
  if (check(type)){
    advance();
    return true;
  }
  return false;
}

/** Verifivca e consome se qualquer um dos tipos corresponder
 * @param types Lista de tipos possiveis
 * @return true se algumn token foi consumido
 */

bool Parser::match(const std::initializer_list<TokenType>& types){
  for (TokenType type : types){
    if (check(type)){
      advance();
      return true;
    }
  }
  return false;
}

/** Consume obrigatoriamente um token do tipo especificado
 * @param type Tipo esperado
 * @param message Mensagem de erro se não corresponder
 * @return Token consumido
 * @throws ParseError se o tipo não corresponder
 */
const Token& Parser::consume(TokenType type, const std::string& message){
  if (check(type)) return advance();
  throw error(peek(), message);

}

/** Cria um objeto de erro de parsing
 * @param token Token onde ocorreu o error
 * @param message Descriçaõ do erro
 * @return Objeto ParseError
 */
ParseError Parser::error(const Token& token, const std::string& message) const{
  return ParseError(token, "[Linha"+ std::to_string(token.line)+ "]" + message);

}

/** Sincronoza após um erro - avança até encontrar um ponto de recuperação 
 * Pontos de recuperação: inicios de declarações( class, func, var, etc.)
 */
void Parser::synchronize(){
  advance();//Avança pelo token problematico

  //Continua avançado até achar um ponto de Sincronozaçaõ 
  while(!isAtEnd()){
    if (precious().type == TokenType::SEMICOLON) return;

    switch (peek().type){
       case TokenType::CLASSE;
       case TokenType::FONCTION;
       case TokenType::LAISSER;
       case TokenType::CONSTANTE;
       case TokenType::SI;
       case TokenType::POUR;
       case TokenType::TANTQUE;
       case TokenType::RETOURNER;
             return;
       default:
             advance();

    }
  }

}

//=========================================================================
//Métodos de parsing de declarações(top-level)
//========================================================================
//
/** Método principal que inicia o parsing do programa
 * @return AST do programa (nó raiz do Program)
 */
std::unique_ptr<Program> Parser::parse(){
  auto program = std::make_unique<Program>();

  //Processa todas as declarações até o final do arquivo]
  while (!isAtEnd()){
    try{
      program->declarations.push_back(parseDeclaration());
    }catch ( const ParseError& e){
      //reporta o erro e Sincronoza para tentat Continuar
      std::cerr << e.what() << std::end1;
      synchronize();

      //Em produção, coletaria todos os erros antes de abortar
      throw;

      }
    }
    return program;
  
}

/** Parseia uma declaraçaõ de alto nivel
 * @return Declaração (class, func, var ou statement)
 */

Dec1Ptr Parser::parseDeclaration(){
  if (match(TokenType::CLASSE))   return parseClasseDeclaration();
  if (match(TokenType::FONCTION)) return parseFunctionDeclaration();
  if (match({TokenType::LAISSER, TokenType::CONSTANTE})){
  //Retrocede pois o token já foi consumido pelo match 
     current_--;
     return parseVarDeclaration();
  }
  return parseStatement();
}


/** Parsei declaraçaõ de classe 
 * Formato: "classe" ID [herite ID]{{método}}
 */
std::unique_ptr<ClassDec1> Parser::parseClasseDeclaration(){
  Token name = consume(TokenType::IDENTIFIER, "Esperado nome da classe após 'classe'");
  //Herança opcional
  std::unique_ptr<VariableExpr> superclass = nullptr;
  if ( match(TokenType::HERITE)){
      Token superName = consume(TokenType::IDENTIFIER, "Esperado nome da superclasse após 'heritee'");
      superclass = std::make_unique<VariableExpr>(superName);
  }
  consume(TokenType::LEFT_BRACE,"Esperado '{'antes do corpo da classe");
  
  //Parser métodos da classe 
  std::vector<std::unique_ptr<VariableExpr>
}

std::unique_ptr,FunctionDec1> Parser::parseFunctionDeclaration(){
   Token name = consume(TokenType::IDENTIFIER, "Expect function name after 'fonction',.");
   consume(TokenType::LEFT_PAREN, "Expect '(' after fnuction name.");

   std::vector<Token> paramns;
   if (!check(TokenType::RIGHT_PAREN)){
     do{
       if (params.size() >= 255){
         reportError(peek(), "Can't have more than 255 parameters.");
       }
       Token param = consume(TokenType::IDENTIFIER, "Expect parameter name");
       params.std::push_back(param);
     }while (match(TokenType::COMMA));
   }
   consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters");

   //Optonal return type
   std::unique_ptr<Type> returnType = nullptr;
   if (match(TokenType::COLON)){
     returnType = parseType();
   }
    
   //Parse functional body
   consume(TokenType::LEFT_BRACE, "Expect '{' before function body.");
   auto body = parseBlock();

   return std::make_unique<FunctionDec1>(name, std::move(params), std::move(returnType), std::move(body));
}

 std::unique_ptr<Type> Parser::parseType() {
    if (match(TokenType::INT)) return std::make_unique<Type>(TypeKind::INT);
    if (match(TokenType::FLOAT)) return std::make_unique<Type>(TypeKind::FLOAT);
    if (match(TokenType::BOOL)) return std::make_unique<Type>(TypeKind::BOOL);
    if (match(TokenType::STRING)) return std::make_unique<Type>(TypeKind::STRING);
    if (match(TokenType::VOID)) return std::make_unique<Type>(TypeKind::VOID);
    
    if (match(TokenType::IDENTIFIER)) {
        return std::make_unique<Type>(TypeKind::CLASS, previous());
    }
    
    if (match(TokenType::LEFT_BRACKET)) {
        auto elementType = parseType();
        consume(TokenType::RIGHT_BRACKET, "Expect ']' after list type.");
        return std::make_unique<Type>(TypeKind::LIST, std::move(elementType));
    }
    
    throw error(peek(), "Invalid type specification");
}

StmtPtr Parser::parseWhileStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'tantque'.");
    auto condition = parseExpression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    
    auto body = parseStatement();
    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

StmtPtr Parser::parseReturnStatement() {
    Token keyword = previous();
    ExprPtr value = nullptr;
    
    if (!check(TokenType::SEMICOLON)) {
        value = parseExpression();
    }
    
    consume(TokenType::SEMICOLON, "Expect ';' after return value.");
    return std::make_unique<ReturnStmt>(keyword, std::move(value));
}

void Parser::reportError(const Token& token, const std::string& message) {
    std::cerr << "[Line " << token.line << "] Error: " << message << "\n";
    hadError = true;
}  

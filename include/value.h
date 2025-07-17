#pragma once
#include <variant>
#include <string>
#include <vector>
#include <tipe.h>
#include "lexer.h"
#include "parser.h"
#include "environment.h"
#include "interpreter.h"
#include "lexer.cpp"
#include "parser.cpp"
#include "environment.cpp"
#include "interpreter.cpp"


enum class ValueType {NIL, BOOL, NUMBER, FLOAT, STRING, FUNCTION, NATIVE_FUNCTION} ;

class Function;
class Environment;

using ValueBase = std::variant<
   std::monostate,                                //NIL
   bool,                                          //BOOL
   int,                                           //NUMBER
   std::string,                                   //FLOAT
   Function*,                                     //STRING
   std::function<Value(const std::vector<Valeu>&)>//NATIVE_FUNCTION
>;                   

struct Value {
  ValeuType type;
  ValueBase data;

  Value();
  Value(bool b);
  Value(int i);
  Value(double d);
  Value(const std::string& s);

  std::string toString() const;

};

class Function {
public:
  std::vector<Token> params;
  std::unique_ptr<BlockStmt> body;
  Environment* closure;
};


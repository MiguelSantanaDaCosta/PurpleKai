#pragma once
#include "lexer.h"
#include <string>

enum class TypeKind {
  INT,
  FLOAT,
  BOOL,
  STRING,
  VOID,
  CLASS,
  LIST,
  OPTIONAL
};

class Type {
public:
  TypeKinf kind;
  Token className;//For CLASS types
  std::unique_ptr<Type> elementType;//For LIST OPTIINAL
  
  //Constructor for simple Type
  explicit Type(TypeKind kind) : kind(kind){}

  //Constructor for class types
  Type(TypeKind kind, Token className)
    : kind(kind), className(className){}
   
  //Constructor for composite type 
  Type(TypeKind kind, std::uniqye_ptr<Type elementType)
    : kind(kind), elementType(std::move(elementType)){}

  std::string toString() const;

};
  

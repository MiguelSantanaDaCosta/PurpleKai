#pragma once
#include "lexer.h"
#include <string>
#include <memory> // Necessário para usar std::unique_ptr

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
  TypeKind kind; // Corrigido de TypeKinf para TypeKind
  Token className; // Para tipos CLASS
  std::unique_ptr<Type> elementType; // Para LIST ou OPTIONAL
  
  // Construtor para tipos simples
  explicit Type(TypeKind kind) : kind(kind) {}

  // Construtor para tipos de classe
  Type(TypeKind kind, Token className)
    : kind(kind), className(className) {}
   
  // Construtor para tipos compostos
  // Corrigido de std::uniqye_ptr<Type para std::unique_ptr<Type>
  Type(TypeKind kind, std::unique_ptr<Type> elementType)
    : kind(kind), elementType(std::move(elementType)) {}

  std::string toString() const;
};

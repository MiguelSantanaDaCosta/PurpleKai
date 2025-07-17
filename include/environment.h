#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "value.h"

class Environment{
  std::unordered_map<std::string, Value> values;
  Environment* parent = nullptr;

  public:
    Environment(Environment* parent = nullptr);
    void define(const std::string& name, Value value);
    Value get(const Token& name);
    void assign(const Token& name, Value value);
};




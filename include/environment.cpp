// include/environment.h
#pragma once

#include <unordered_map>
#include "value.h"

class Environment {
public:
    explicit Environment(std::shared_ptr<Environment> enclosing = nullptr);
    
    void define(const std::string& name, Value value);
    Value get(const Token& name);
    void assign(const Token& name, Value value);
    
private:
    std::unordered_map<std::string, Value> values_;
    std::shared_ptr<Environment> enclosing_;
};

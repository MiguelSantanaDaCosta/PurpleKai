#include "../include/environment.h"
#include <stdexcept>

Environment::Environment(Environment* parent) : parent(parent) {}

void Environment::define(const std::string& name, Value value) {
    values[name] = value;
}

Value Environment::get(const Token& name) {
    if (values.find(name.lexeme) != values.end()) {
        return values[name.lexeme];
    }
    if (parent != nullptr) return parent->get(name);
    throw std::runtime_error("Variável indefinida: " + name.lexeme);
}

void Environment::assign(const Token& name, Value value) {
    if (values.find(name.lexeme) != values.end()) {
        values[name.lexeme] = value;
        return;
    }
    if (parent != nullptr) {
        parent->assign(name, value);
        return;
    }
    throw std::runtime_error("Atribuição a variável indefinida: " + name.lexeme);
}

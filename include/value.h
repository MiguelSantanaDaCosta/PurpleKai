#pragma once

#include <variant>
#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "type.h"
#include "lexer.h"

// Forward declarations
class Environment;
class Interpreter;
class BlockStmt;

struct Value;

enum class ValueType {
    NIL,
    BOOL,
    NUMBER,
    FLOAT,
    STRING,
    FUNCTION,
    NATIVE_FUNCTION
};

class Function;

using ValueBase = std::variant<
    std::monostate,
    bool,
    int,
    double,
    std::string,
    Function*,
    std::function<Value(const std::vector<Value>&)>
>;

struct Value {
    ValueType type;
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
    Environment* closure = nullptr;
};

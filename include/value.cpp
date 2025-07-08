// include/value.h
#pragma once

#include <variant>
#include <string>
#include <memory>

class Object;
class Function;

using Value = std::variant<
    std::monostate,  // nil
    bool,
    int64_t,
    double,
    std::string,
    std::shared_ptr<Object>,
    std::shared_ptr<Function>
>;

class ValueHelper {
public:
    static std::string typeName(const Value& value);
    static Value add(const Value& a, const Value& b);
    // ... outras operações
};

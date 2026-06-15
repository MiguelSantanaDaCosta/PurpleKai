#include "../include/value.h"

Value::Value()
    : type(ValueType::NIL),
      data(std::monostate{})
{
}

Value::Value(bool b)
    : type(ValueType::BOOL),
      data(b)
{
}

Value::Value(int i)
    : type(ValueType::NUMBER),
      data(i)
{
}

Value::Value(double d)
    : type(ValueType::FLOAT),
      data(d)
{
}

Value::Value(const std::string& s)
    : type(ValueType::STRING),
      data(s)
{
}

std::string Value::toString() const
{
    switch (type)
    {
        case ValueType::NIL:
            return "nulle";

        case ValueType::BOOL:
            return std::get<bool>(data) ? "vrai" : "faux";

        case ValueType::NUMBER:
            return std::to_string(std::get<int>(data));

        case ValueType::FLOAT:
            return std::to_string(std::get<double>(data));

        case ValueType::STRING:
            return std::get<std::string>(data);

        case ValueType::FUNCTION:
            return "<fonction>";

        case ValueType::NATIVE_FUNCTION:
            return "<fonction_native>";
    }

    return "<inconnu>";
}

#include "../include/type.h"
#include <string>

std::string Type::toString() const {
    switch (kind) {
        case TypeKind::INT: return "Int";
        case TypeKind::FLOAT: return "Float";
        case TypeKind::BOOL: return "Bool";
        case TypeKind::STRING: return "String";
        case TypeKind::VOID: return "Void";
        case TypeKind::CLASS: return className.lexeme;
        case TypeKind::LIST: return "[" + (elementType ? elementType->toString() : "?") + "]";
        case TypeKind::OPTIONAL: return (elementType ? elementType->toString() : "?") + "?";
        default: return "<unknown>";
    }
}

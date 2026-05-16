#pragma once
#include <core/types.hpp>

inline uint8 idx(ValueType type) {
    return static_cast<uint8>(type);
}

template<typename T>
inline T castTo(const Value& value) {
    return std::visit([](auto&& v) -> T { return static_cast<T>(v); }, value.value);
}

PrimitiveCategory getCategory(ValueType type) {
    switch (type) {
        case ValueType::INT8:
        case ValueType::INT16:
        case ValueType::INT32:
        case ValueType::INT64: return PrimitiveCategory::SIGNED;
        case ValueType::UINT8:
        case ValueType::UINT16:
        case ValueType::UINT32:
        case ValueType::UINT64: return PrimitiveCategory::UNSIGNED;
        case ValueType::FLOAT32:
        case ValueType::FLOAT64: return PrimitiveCategory::FLOATING;
        default: return PrimitiveCategory::UNKNNOWN;
    }
}

template<typename T, typename U>
bool compareValuesOp(T operand1, U operand2, Index operatorIndex) {
    switch (static_cast<OperatorCategory>(operatorIndex)) {
        case OperatorCategory::EQUALS: return operand1 == operand2;
        case OperatorCategory::BIGGER: return operand1 > operand2;
        case OperatorCategory::BIGGEREQ: return operand1 >= operand2;
        case OperatorCategory::SMALLER: return operand1 < operand2;
        case OperatorCategory::SMALLEREQ: return operand1 <= operand2;
        default: return false;
    }
}

bool compareValuesVariant(const Value& operand1, const Value& operand2, Index operatorIndex) {
    return std::visit([&](auto&& value1) {
        return std::visit([&](auto&& value2) {
            return compareValuesOp(value1, value2, operatorIndex);
        }, operand2.value);
    }, operand1.value);
}

// inline ValueContainer defaultOf(ValueType type) {
//     switch (type) {
//         case ValueContainer::
//     }
// }

bool checkInBlock(Index codeStart, Index codeEnd, Index position) {
    return position >= codeStart && position < codeEnd;
}
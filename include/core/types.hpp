#pragma once
#include <variant>
#include <cstdint>

// primitive types
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using float32 = float;
using float64 = double;

// using unichar = char8_t;

// system types
// using varray = uint16;
// using vstruct = uint16;

// using vvariable = uint16;
// using vreference = uint16;
// using vfunction = uint16;
// class Index {
// public:
//     Index() : value(0) {}
//     Index(UInt32 value) : value(value) {}

//     operator UInt32() { return value; }
    
//     bool operator>(const Index& obj) { return value > obj.value; }
//     bool operator<(const Index& obj) { return value < obj.value; }

//     bool operator==(const Index& obj) { return value == obj.value; }
//     bool operator!=(const Index& obj) { return value != obj.value; }

//     Index& operator++() {
//         value++;

//         return *this;
//     }

//     Index operator++(int) {
//         Index tmp = *this;

//         value++;

//         return tmp;
//     }

// private:
//     UInt32 value;
// };

using Index = int32;
using Size = int32;


// struct vdummy {};

// using vtypeid = uint16; // metatype id

// type enum
enum class ValueType : uint8 {
    INT8,
    INT16,
    INT32,
    INT64,
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    FLOAT32,
    FLOAT64,
    BOOL,
    NAME,
    TYPE,
    CONST,
    VALUE,
    VARIABLE,
    LAMBDA,
    REFERECNE,
    STRUCT,
    ARRAY,
    NONE
};

enum class PrimitiveCategory : uint8 {
    SIGNED,
    UNSIGNED,
    FLOATING,
    BOOL,
    UNKNNOWN
};

enum class OperatorCategory : uint8 {
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    BIGGER,
    EQUALS,
    BIGGEREQ,
    SMALLER,
    SMALLEREQ
};

// main type container
using ValueContainer = std::variant<int8, int16, int32, int64, uint8, uint16, uint32, uint64, float32, float64, bool>;

struct Value {
    Index type; // type ref
    ValueContainer value;
};


// struct ValueIndex {
//     Index type; // type ref
//     Index value; // value ref
// };

// enum class IndexType { // only for instructions
//     NONE,
    
// };


// // internal type aliases
// using vistring = std::string;

// template<typename Ty>
// using viarray = std::vector<Ty>;

// template<typename Ty>
// using vistack = std::stack<Ty>;

// template<typename Tk, typename Tv>
// using vihashmap = std::unordered_map<Tk, Tv>;

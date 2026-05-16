#pragma once
#include <core/types.hpp>

struct ValueEntry {
    Value value;
    Index refIndex;
    bool bound;
    bool initialized;
};
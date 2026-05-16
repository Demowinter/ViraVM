#pragma once
#include <core/types.hpp>

struct Exception {
    bool active;

    Index typeIndex;
    Index valueIndex;

    ValueType exceptionType;
};
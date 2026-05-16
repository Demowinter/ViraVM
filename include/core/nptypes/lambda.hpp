#pragma once
#include <vector>
#include <core/types.hpp>

struct LambdaEntry {
    Index functionMeta; // function ref

    std::vector<Index> capturedValues;
};
#pragma once
#include <vector>
#include <core/types.hpp>

struct ArrayEntry {
    Index arrayMeta; // array ref
    std::vector<Index> valueArray; // value ref
};
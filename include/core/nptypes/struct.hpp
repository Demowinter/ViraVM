#pragma once
#include <vector>
#include <core/types.hpp>

struct StructEntry {
    Index structMeta; // struct ref | reference to struct layout
    std::vector<Index> memberTable; // index immutable | value ref
};
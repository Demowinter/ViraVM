#pragma once
#include <core/types.hpp>

struct ReferenceEntry {
    Index trueTypeMeta; // type ref | type of the true value
    Index value; // value ref
    uint16 count;
    bool alive;
};
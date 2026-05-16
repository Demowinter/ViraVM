#pragma once
#include <core/types.hpp>

struct MetaTypeEntry {
    Index typeName; // name ref
    ValueType typeBinding;
    Index subtypeBinding; // type ref
    Index externalTypeBinding; // struct/function ref | only for building entries
};
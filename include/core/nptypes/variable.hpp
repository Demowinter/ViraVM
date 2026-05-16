#pragma once
#include <core/types.hpp>

struct VariableEntry {
    Index variableMeta; // global/local variable ref
    Index value; // value ref
};
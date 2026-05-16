#pragma once
#include <vector>
#include <core/types.hpp>

struct MetaStructMemberEntry {
    Index memberName; // name ref
    Index memberType; // type ref
};

struct MetaStructEntry {
    std::vector<MetaStructMemberEntry> memberPool;
};
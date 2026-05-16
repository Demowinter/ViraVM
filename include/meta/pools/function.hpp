#pragma once
#include <vector>
#include <core/types.hpp>
#include <meta/pools/variable.hpp>

struct MetaCatchBlockEntry {
    Index codeStart;
    Index codeEnd;

    Index catchArg; // type ref
};

struct MetaTryBlockEntry {
    Index codeStart;
    Index codeEnd;
    
    std::vector<MetaCatchBlockEntry> catchBlockPool;
};

struct MetaFunctionEntry {
    Index functionName; //name ref

    std::vector<MetaVariableEntry> localVarPool;
    std::vector<MetaTryBlockEntry> tryBlockPool;

    Index codeStart;
    Index codeEnd;

    Index returnType; // type ref
};
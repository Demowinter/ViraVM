#pragma once
#include <vector>
#include <core/types.hpp>
#include <core/exception.hpp>
#include <core/nptypes/variable.hpp>

struct CallFrame {
    Index functionMeta; // function ref

    std::vector<VariableEntry> localVarTable;

    Exception exception;

    Index returnStackAddress;
    Index returnAddress;
};
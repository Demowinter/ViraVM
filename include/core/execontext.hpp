#pragma once
#include <vector>
#include <map>
#include <core/types.hpp>
#include <core/callframe.hpp>
#include <core/nptypes/value.hpp>
#include <core/nptypes/array.hpp>
#include <core/nptypes/struct.hpp>
#include <core/nptypes/variable.hpp>
#include <core/nptypes/reference.hpp>
#include <core/nptypes/lambda.hpp>
#include <meta/pools/name.hpp>
#include <meta/pools/type.hpp>
#include <meta/pools/constant.hpp>
#include <meta/pools/variable.hpp>
#include <meta/pools/function.hpp>
#include <meta/pools/array.hpp>
#include <meta/pools/struct.hpp>
#include <bytecode/header.hpp>

struct ExecutionContext {
    // Codegen/bytecode info | Immutable
    BytecodeHeader codeHeader;

    // Metadata | Immutable
    std::vector<MetaNameEntry> namePool;
    std::vector<MetaTypeEntry> typePool;
    std::vector<MetaConstantEntry> constPool;
    std::vector<MetaVariableEntry> globalVarPool;
    std::vector<MetaFunctionEntry> functionPool;
    std::vector<MetaArrayEntry> arrayPool;
    std::vector<MetaStructEntry> structPool;

    // Runtime data | Mutable | index immutable
    std::vector<VariableEntry> globalVarTable;

    // Runtime data | Mutable | index mutable
    std::map<Index, ValueEntry> valueTable;
    std::map<Index, ReferenceEntry> referenceTable;
    std::map<Index, LambdaEntry> lambdaTable;
    std::map<Index, ArrayEntry> arrayTable;
    std::map<Index, StructEntry> structTable;

    std::vector<CallFrame> callStack;
    std::vector<Index> valueStack;

    Index instructionPointerNext;
    Index instructionPointer;
    int8 exitCode;
};
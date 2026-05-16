#pragma once
#include <string>
#include <core/types.hpp>
#include <core/execontext.hpp>
#include <core/isa.hpp>

enum class LoadStatus : int8 {
    OK,
    FILE_NOT_FOUND,
    FILE_OPEN_ERROR,
    CORRUPTED_FILE,
    UNSUPPORTED_VERSION,
    UNKNOWN_ERROR
};

struct LoadResult {
    LoadStatus status;
    ExecutionContext context;
    InstructionList list;
};

LoadResult loadBytecode(std::string path);
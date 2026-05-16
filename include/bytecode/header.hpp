#pragma once
#include <string>
#include <core/types.hpp>

struct BytecodeHeader {
    uint32 magic;
    
    uint8 bytecodeVersionMajor;
    uint8 bytecodeVersionMinor;

    uint8 codegenVersionMajor;
    uint8 codegenVersionMinor;
    std::string codegenName;
};
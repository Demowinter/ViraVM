#pragma once
#include <string>
#include <core/types.hpp>

constexpr uint8 vmVersionMajor = 0;
constexpr uint8 vmVersionMinor = 1;

inline const std::string vmNameString = "ViraVM";

constexpr uint32 bytecodeMagic = 0xF0BD38AA;
constexpr uint8 bytecodeVersionMajor = 0;
constexpr uint8 bytecodeVersionMinor = 1;

constexpr Index invalidIndex = -1;

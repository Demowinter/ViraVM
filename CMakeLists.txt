cmake_minimum_required(VERSION 3.10)
project(victim VERSION 0.1)

include_directories("include/")

add_executable(victim-devel src/main.cpp src/core/runner.cpp src/core/runtime.cpp src/core/isa.cpp src/bytecode/loader.cpp src/test/testmaker.cpp)
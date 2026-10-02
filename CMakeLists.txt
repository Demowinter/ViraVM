cmake_minimum_required(VERSION 3.10)
project(viravm VERSION 0.1)

include_directories("include/")

add_executable(viravm-devel src/main.cpp src/core/runner.cpp src/core/runtime.cpp src/core/isa.cpp src/bytecode/loader.cpp src/test/testmaker.cpp)

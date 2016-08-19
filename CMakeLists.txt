cmake_minimum_required (VERSION 2.6)
add_definitions(-std=c++11)

set(BIN_PREFIX bin/)

project (fuzzer)
add_executable(${BIN_PREFIX}fuzzer fuzzer.cpp binary_runner.cpp)
add_executable(${BIN_PREFIX}test_bin test_bin.cpp)

cmake_minimum_required (VERSION 2.6)

set(BIN_PREFIX bin/)

project (fuzzer)
add_executable(${BIN_PREFIX}fuzzer fuzzer.cpp)

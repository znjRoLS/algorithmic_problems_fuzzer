cmake_minimum_required (VERSION 2.6)
add_definitions(-std=c++11)

set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#set(BIN_PREFIX ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

project (fuzzer)

add_executable(fuzzer fuzzer.cpp binary_runner.cpp common.cpp config.cpp block.cpp block.h variable.cpp variable.h)
#add_executable(test_bin test_bin.cpp)

include_directories(googletest/googletest/include)
add_subdirectory (googletest/googletest)
enable_testing()

cxx_test(variable_test gtest_main variable.cpp)
cxx_test(block_test gtest_main block.cpp variable.cpp)
cxx_test(config_test gtest_main config.cpp block.cpp variable.cpp common.cpp)


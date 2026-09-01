cmake_minimum_required(VERSION 3.20)

project(NucleusDB
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Main database executable
add_executable(nucleusdb
    src/main.cpp
)

# GoogleTest
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

# Tests
enable_testing()

add_executable(nucleusdb_tests
    tests/lexer/token_test.cpp
)

target_link_libraries(nucleusdb_tests
    PRIVATE
    GTest::gtest_main
)

target_include_directories(nucleusdb_tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

include(GoogleTest)

gtest_discover_tests(nucleusdb_tests)
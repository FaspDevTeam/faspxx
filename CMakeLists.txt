# Main CMake file for FASP++
# Some smaple usages:
#   cmake -DCMAKE_VERBOSE_MAKEFILE=ON // To make cmake verbose
#   cmake -DCMAKE_BUILD_TYPE=Debug    // To build Debug target
#   cmake -DCMAKE_BUILD_TYPE=Release  // To build Release target

# Minimum cmake version needed
cmake_minimum_required(VERSION 2.8)

# Enforce language standard C++11
set(CMAKE_CXX_STANDARD 11)

find_program(THE_C NAMES gcc icc clang)
set(CMAKE_C_COMPILER ${THE_C} CACHE INTERNAL "the C compiler" FORCE)

find_program(THE_CXX NAMES g++ icc clang)
set(CMAKE_CXX_COMPILER ${THE_CXX} CACHE INTERNAL "the CXX compiler" FORCE)

# Configuration types
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -funroll-loops")
# message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
# message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")

# Setup project name
project(faspxx)

# Add include directory
include_directories(include/
                    src)

# Add source code from src
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cxx)
add_library(faspxx ${SRC_FILES})

# Test error code class
add_executable(testErrorCode test/testErrorCode.cxx)
target_link_libraries(testErrorCode faspxx)

# Test vector operations
add_executable(testVecSpeed test/testVecSpeed.cxx)

# Test VEC class operations
add_executable(testVEC test/testVEC.cxx)
target_link_libraries(testVEC faspxx)

# Test Google Test environment
# add_executable(testGoogle test/testGoogle.cxx)
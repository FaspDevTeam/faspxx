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

# Find source code from src
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cxx)

# Build faspxx library
add_library(faspxx ${SRC_FILES})

# Build error-code test
add_executable(testErrorCode test/testErrorCode.cxx)
target_link_libraries(testErrorCode faspxx)

# Build vector-operation test
add_executable(testVecSpeed test/testVecSpeed.cxx)

# Build VEC class test
add_executable(testVEC test/testVEC.cxx)
target_link_libraries(testVEC faspxx)

# Build GTest test
enable_testing()
find_package(GTest REQUIRED)
message("${GTEST_INCLUDE_DIRS}")
message("${GTEST_LIBRARIES}")
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(testGoogle test/testGoogle.cxx)
target_link_libraries(testGoogle ${GTEST_LIBRARIES})
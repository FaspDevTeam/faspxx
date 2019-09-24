# Main CMake file for FASP++
# Some smaple usages:
#   cmake -DCMAKE_VERBOSE_MAKEFILE=ON // To make cmake verbose
#   cmake -DCMAKE_BUILD_TYPE=Debug    // To build Debug target
#   cmake -DCMAKE_BUILD_TYPE=Release  // To build Release target

# Minimum cmake version needed
cmake_minimum_required(VERSION 2.8)

# Set C++ language standard to ANSI C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default C++ compiler and flags
find_program(THE_CXX NAMES $ENV{CXX} clang++ g++ icc)
set(CMAKE_CXX_COMPILER ${THE_CXX} CACHE INTERNAL "the CXX compiler" FORCE)

# Set default C compiler order and flags
find_program(THE_C NAMES $ENV{CC} clang gcc icc)
set(CMAKE_C_COMPILER ${THE_C} CACHE INTERNAL "the C compiler" FORCE)

# Set configuration types
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Set project name
project(faspxx)

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include/
                    ${PROJECT_SOURCE_DIR}/src/)

# Find source code from src
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cxx)

# Build faspxx library
add_library(faspxx ${SRC_FILES})

set(CMAKE_INSTALL_LIBDIR "${PROJECT_SOURCE_DIR}/lib/")
install(TARGETS faspxx
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # added by fan
        )

# Build error-code test
add_executable(testErrorCode test/testErrorCode.cxx)
target_link_libraries(testErrorCode faspxx)

# Build vector-operation test
add_executable(testVecSpeed test/testVecSpeed.cxx)
target_link_libraries(testVecSpeed faspxx)

# Build VEC class test
#add_executable(testVEC test/testVEC.cxx)
#target_link_libraries(testVEC faspxx)
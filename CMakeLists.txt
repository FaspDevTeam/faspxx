# Main CMake file for FASP++
# by Chensong Zhang, modified on Sep/26/2019
#
# Some smaple usages:
#   cmake -DCMAKE_VERBOSE_MAKEFILE=ON . // build . dir with verbose
#   cmake -DCMAKE_BUILD_TYPE=Debug .    // build . in Debug configuration
#   cmake -DCMAKE_BUILD_TYPE=Release .  // build . in Release configuration

#-----------------------------#
# General environment setting #
#-----------------------------#

# Minimum cmake version needed
cmake_minimum_required(VERSION 2.8)

# Set C++ language standard (ANSI C++ 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default C++ compiler look-up order
find_program(THE_CXX NAMES $ENV{CXX} g++ icc clang++)
set(CMAKE_CXX_COMPILER ${THE_CXX} CACHE INTERNAL "the CXX compiler" FORCE)

# Default C compiler look-up order
find_program(THE_C NAMES $ENV{CC} gcc icc clang)
set(CMAKE_C_COMPILER ${THE_C} CACHE INTERNAL "the C compiler" FORCE)

# Set default configuration types and flags
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

#-----------------------------#
# Project specific parameters #
#-----------------------------#

# Name of the project
project(faspxx CXX C)

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(${PROJECT_SOURCE_DIR}/example/)

# Find source code from src
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cxx)

# Build faspxx library
add_library(faspxx ${SRC_FILES})

set(CMAKE_INSTALL_LIBDIR "${PROJECT_SOURCE_DIR}/lib/")
install(TARGETS faspxx
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Build vector-operations speed test
add_executable(testVecSpeed example/testVecSpeed.cxx)
target_link_libraries(testVecSpeed faspxx)

# Build ReadData test
add_executable(testReadData example/testReadData.cxx)
target_link_libraries(testReadData faspxx)

# Build PCG class test
add_executable(testPCG example/testPCG.cxx)
target_link_libraries(testPCG faspxx)

add_executable(testError example/testError.cxx)
target_link_libraries(testError faspxx)

add_executable(testMatFree example/testMatFree.cxx)
target_link_libraries(testMatFree faspxx)

add_executable(testBiCGStab example/testBiCGStab.cxx)
target_link_libraries(testBiCGStab faspxx)

# Enable or Disable GoogleTest, default "ON" (disable by "-DGTest=OFF")
option(GTEST "Enable GoogleTest" ON)
if ("${GTEST}" STREQUAL ON)
    add_subdirectory(utest)
endif()

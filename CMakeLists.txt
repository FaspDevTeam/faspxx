# Main CMakeLists file for FASP++
# by Chensong Zhang, modified on Feb/15/2020
#
# Some sample usages:
#   cmake .                                      // build in default configuration
#   cmake -DCMAKE_BUILD_TYPE=Release .           // build in Release configuration
#   cmake -DCMAKE_BUILD_TYPE=Debug .             // build in Debug configuration
#   cmake -DCMAKE_BUILD_TYPE=Debug -DGTEST=ON .  // build with GTEST support
#   cmake -DUSE_UMFPACK=ON .                     // build with UMFPACK support
#   cmake -DCMAKE_VERBOSE_MAKEFILE=ON .          // build with verbose on

#-----------------------------#
# General environment setting #
#-----------------------------#

# Minimum cmake version needed
cmake_minimum_required(VERSION 2.8.12)

# Set C++ language standard (ANSI C++ 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default C++ compiler look-up order
find_program(THE_CXX NAMES $ENV{CXX} g++ icpc clang++)
set(CMAKE_CXX_COMPILER ${THE_CXX} CACHE INTERNAL "the CXX compiler" FORCE)

# Default C compiler look-up order
find_program(THE_C NAMES $ENV{CC} gcc icc clang)
set(CMAKE_C_COMPILER ${THE_C} CACHE INTERNAL "the C compiler" FORCE)

# Default configuration types and flags
set(CMAKE_CONFIGURATION_TYPES "Release;Debug" CACHE STRING "Configs" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")

set(build_type "Release")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "${build_type}" CACHE STRING "the build type" FORCE)
endif()
message(STATUS "Set build type to ${CMAKE_BUILD_TYPE} configuration")

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
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/modules)

# Set install location for faspxx library
set(CMAKE_INSTALL_LIBDIR "${PROJECT_SOURCE_DIR}/lib/")
install(TARGETS faspxx
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Look for external libraries (Optional)
include(external)

# Test vector-operations speed
add_executable(testVecSpeed example/testVecSpeed.cxx)
target_link_libraries(testVecSpeed faspxx)

# Test ReadData
add_executable(testReadData example/testReadData.cxx)
target_link_libraries(testReadData faspxx)

# Test direct solvers
add_executable(testSolver example/testSolver.cxx)
target_link_libraries(testSolver faspxx ${UMFPACK_LIBRARIES})

# Test AMG class
add_executable(testAMG example/testAMG.cxx)
target_link_libraries(testAMG faspxx)

# Test CG class
add_executable(testCG example/testCG.cxx)
target_link_libraries(testCG faspxx)

# Test PCG class
add_executable(testPCG example/testPCG.cxx)
target_link_libraries(testPCG faspxx)

# Test GMRES class
add_executable(testGMRES example/testGMRES.cxx)
target_link_libraries(testGMRES faspxx)

# Test Krylov methods
add_executable(testKrylov example/testKrylov.cxx)
target_link_libraries(testKrylov faspxx)

# Test Jacobi method
add_executable(testJacobi example/testJacobi.cxx)
target_link_libraries(testJacobi faspxx)

# Test Weighted Jacobi method
add_executable(testWeightedJacobi example/testWeightedJacobi.cxx)
target_link_libraries(testWeightedJacobi faspxx)

# Test MatFree implementation of 2D5P stencil
add_executable(testInverse example/testInverse.cxx)
target_link_libraries(testInverse faspxx)

# Test MatFree implementation of 2D5P stencil
add_executable(testMatFree example/testMatFree.cxx)
target_link_libraries(testMatFree faspxx)

# GoogleTest, default "OFF" (enable by "-DGTEST=ON")
option(GTEST "Google Test" OFF)
add_subdirectory(utest)
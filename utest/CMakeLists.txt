## -------------------------------------------- GoogleTest Independent from FASP++ --------------------------------------
## Unit Test CMake file for FASP++
## Some smaple usages:
##   cmake -DCMAKE_VERBOSE_MAKEFILE=ON // To make cmake verbose
##   cmake -DCMAKE_BUILD_TYPE=Debug    // To build Debug target
##   cmake -DCMAKE_BUILD_TYPE=Release  // To build Release target
#
## Minimum cmake version needed
#cmake_minimum_required(VERSION 2.8)
#
## Set C++ language standard to ANSI C++11
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
#
## Set default C++ compiler and flags
#find_program(THE_CXX NAMES $ENV{CXX} clang++ g++ icc)
#set(CMAKE_CXX_COMPILER ${THE_CXX} CACHE INTERNAL "the CXX compiler" FORCE)
#
## Set default C compiler order and flags
#find_program(THE_C NAMES $ENV{CC} clang gcc icc)
#set(CMAKE_C_COMPILER ${THE_C} CACHE INTERNAL "the C compiler" FORCE)
#
## Set configuration types
#SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -funroll-loops")
#
## Set project name
#project(faspxx_utest)
#
## Set FASP++ location
#SET(FASPXX_DIR "${PROJECT_SOURCE_DIR}/.." CACHE PATH "FASP++ directory override" FORCE)
#
## Set required packages: GTest
#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})
#
## Set required packages: FASP++ library
#link_directories("${FASPXX_DIR}/lib/")
#
## Add include directory
#include_directories(${FASPXX_DIR}/include/
#                    ${FASPXX_DIR}/src/)
#
## Find source code from src
##file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cxx)
#
### Build GTest test
##add_executable(unitGTest unitGTest.cxx)
##target_link_libraries(unitGTest ${GTEST_LIBRARIES} faspxx)
##
### Build VEC class test
##add_executable(unitVEC unitVEC.cxx)
##target_link_libraries(unitVEC ${GTEST_LIBRARIES} faspxx)
#
## Test All at once
#add_executable(testAll unitGTestAll.cxx)
#target_link_libraries(testAll ${GTEST_LIBRARIES} faspxx pthread) # add pthread by fan



# -------------------------------------------- GoogleTest Dependent from FASP++ --------------------------------------
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(testAll unitGTestAll.cxx)
target_link_libraries(testAll "${GTEST_LIBRARIES}" pthread faspxx)

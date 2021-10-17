# Main CMakeLists file for FASP++
# by Chensong Zhang, modified on Feb/15/2020
# by Ronghong Fan & Chensong Zhang, modified on Oct/10/2021
#
# Some sample usages:
#   cmake .                                      // build in default configuration
#   cmake -DCMAKE_BUILD_TYPE=Release .           // build in Release configuration
#   cmake -DCMAKE_BUILD_TYPE=Debug .             // build in Debug configuration
#   cmake -DCMAKE_BUILD_TYPE=Debug -DUTEST=ON .  // build with unit tests
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
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(${PROJECT_SOURCE_DIR}/example/)

# CMake modules path
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/modules)
include(UserCmakeUtilities)

# Look for external libraries (Optional)
include(external)
find_package(MPI REQUIRED)
if(MPI_FOUND)
    include_directories(${MPI_CXX_INCLUDE_DIRS})
    link_libraries(${MPI_LIBRARIES})
endif()

# Build faspxx library
set(SOURCES "")
set(HEADERS "")
set(FASPXX_SOURCE_DIRS src) # list of src directories
foreach(DIR IN LISTS FASPXX_SOURCE_DIRS)
    add_subdirectory(${DIR})
endforeach()
add_library(faspxx ${SOURCES} ${HEADERS})

# Set install location for faspxx library
set(CMAKE_INSTALL_LIBDIR "${PROJECT_SOURCE_DIR}/lib/")
install(TARGETS faspxx
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_subdirectory(example)

# Unit test using Catch2
option(UTEST "Unit Test" OFF)
if(UTEST) 
    enable_testing()
    add_subdirectory(tests)
endif()
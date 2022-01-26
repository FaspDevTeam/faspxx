# Main CMakeLists file for FASP++
# by Chensong Zhang, modified on Feb/15/2020
# by Ronghong Fan & Chensong Zhang, modified on Oct/15/2021
# by Chensong Zhang, modified on Jan/22/2022: Add OMP, remove C
#
# Some sample usages:
#   cmake <DIR>                                      // default configuration
#   cmake <DIR> -DCMAKE_BUILD_TYPE=Debug             // debug configuration
#   cmake <DIR> -DCMAKE_BUILD_TYPE=Debug -DUTEST=ON  // debug with unit tests
#   cmake <DIR> -DUSE_OPENMP=ON .                    // with OpenMP support
#   cmake <DIR> -DUSE_UMFPACK=ON .                   // with UMFPACK solvers
#   cmake <DIR> -DCMAKE_VERBOSE_MAKEFILE=ON .        // with verbose on

#-----------------------------#
# General environment setting #
#-----------------------------#

# Minimum cmake version needed
cmake_minimum_required(VERSION 3.0)

# Set C++ language standard (ANSI C++ 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default C++ compiler look-up order
find_program(THE_CXX NAMES $ENV{CXX} g++ icpc clang++)
set(CMAKE_CXX_COMPILER ${THE_CXX} CACHE INTERNAL "the CXX compiler" FORCE)

# Default compiler flags
set(CMAKE_CONFIGURATION_TYPES "Release;Debug" CACHE STRING "Configs" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")

# Default configuration 
set(build_type "Release")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "${build_type}" CACHE STRING "the build type" FORCE)
endif()
message(STATUS "Set build type to ${CMAKE_BUILD_TYPE} configuration")

#-----------------------------#
# Project specific parameters #
#-----------------------------#

# Name of the project
project(faspxx CXX)

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(${PROJECT_SOURCE_DIR}/example/)

# CMake modules path
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/modules)
include(UserCmakeUtilities)

# OpenMP : defined on command line in the top Makefile
if(USE_OPENMP)
    find_package(OpenMP)
    if(OPENMP_FOUND)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    else(OPENMP_FOUND)
        message("   WARNING: OpenMP was disabled!")
    endif(OPENMP_FOUND)
endif(USE_OPENMP)

# Build faspxx library
set(SOURCES "")
set(HEADERS "")
set(FASPXX_SOURCE_DIRS src) # list of src directories
foreach(DIR IN LISTS FASPXX_SOURCE_DIRS)
    add_subdirectory(${DIR})
endforeach()
add_library(faspxx ${SOURCES} ${HEADERS})

# Set location for installing faspxx library
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}" )
install(TARGETS faspxx
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
set(FASPXX_INSTALL TRUE)

# Set faspxx library type
if(SHARED) 
    set(FASPXX_LIBRARY_TYPE SHARED)
else(SHARED)
    set(FASPXX_LIBRARY_TYPE STATIC)
endif(SHARED)
set(FASPXX_LIBRARY_NAME faspxx)

# Uninstall target
if (NOT TARGET uninstall)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/modules/cmake_uninstall.cmake.in"
                   "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                   IMMEDIATE @ONLY)
    add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P 
                      ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

# Look for external libraries (Optional)
include(external)

add_subdirectory(example)

# Unit test using Catch2
option(UTEST "Unit Test" OFF)
if(UTEST) 
    enable_testing()
    add_subdirectory(utest)
endif()
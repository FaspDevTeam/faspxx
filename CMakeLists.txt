# Main CMakeLists file for FASP++
# by Chensong Zhang, modified on Feb/15/2020
# by Ronghong Fan & Chensong Zhang, modified on Oct/15/2021
# by Chensong Zhang, modified on Jan/22/2022: Add OMP, remove C
# by Xiaoxing Cheng, modified on Nov/15/2024: Update CMake file to modern cmake
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
cmake_minimum_required(VERSION 3.13)

# Set C++ language standard (ANSI C++ 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default C++ compiler look-up order
find_program(THE_CXX NAMES $ENV{CXX} g++ icpc icpx clang++)
set(CMAKE_CXX_COMPILER ${THE_CXX} CACHE INTERNAL "the CXX compiler" FORCE)


# Default configuration 
set(build_type "Release")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "${build_type}" CACHE STRING "the build type" FORCE)
endif()
message(STATUS "Set build type to ${CMAKE_BUILD_TYPE} configuration")

#-----------------------------#
# Project specific parameters #
#-----------------------------#

project(faspxx LANGUAGES CXX VERSION 0.1.0)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/modules)
include(UserCmakeUtilities)

add_library(faspxx)
add_library(FASP::faspxx  ALIAS faspxx)
add_subdirectory(src)

set_target_properties(faspxx PROPERTIES
    COMPILE_FLAGS_RELEASE "-O2 -march=native"
)

# Set debug flags for faspxx library
set_target_properties(faspxx PROPERTIES
    COMPILE_FLAGS_DEBUG "-Wall -g"
)

target_include_directories(faspxx
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/  # If other targets need these headers when linking to faspxx
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/
)

# CMake modules path

# OpenMP : defined on command line in the top Makefile
if(USE_OPENMP)
    find_package(OpenMP)
    if(OPENMP_FOUND)
        target_compile_options(faspxx PRIVATE ${OpenMP_CXX_FLAGS})
        target_link_libraries(faspxx PRIVATE OpenMP::OpenMP_CXX)
    else(OPENMP_FOUND)
        message("   WARNING: OpenMP was disabled!")
    endif(OPENMP_FOUND)
endif(USE_OPENMP)



# Set location for installing faspxx library


# Set faspxx library type
if(SHARED) 
    set(FASPXX_LIBRARY_TYPE SHARED)
else(SHARED)
    set(FASPXX_LIBRARY_TYPE STATIC)
endif(SHARED)
set(FASPXX_LIBRARY_NAME faspxx)

add_subdirectory(example)
# Unit test using Catch2
option(UTEST "Unit Test" OFF)
if(UTEST) 
    enable_testing()
    add_subdirectory(utest)
endif()

# Uninstall target
if (NOT TARGET uninstall)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/modules/cmake_uninstall.cmake.in"
                   "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                   IMMEDIATE @ONLY)
    add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P 
                      ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

target_include_directories(faspxx PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

include(GNUInstallDirs)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR} CACHE PATH "the install path" FORCE)
install(TARGETS faspxx
        EXPORT faspxxTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
set(FASPXX_INSTALL TRUE)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    faspxxConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )
install(EXPORT faspxxTargets
    FILE faspxxTargets.cmake
    NAMESPACE FASP::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/faspxx
     )
# export(TARGETS faspxx NAMESPACE FASP:: FILE ${PROJECT_SOURCE_DIR}/modules/faspxx-targets.cmake)
configure_file(${PROJECT_SOURCE_DIR}/modules/faspxxConfig.cmake.in faspxxConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/faspxxConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/faspxxConfigVersion.cmake"
        DESTINATION {CMAKE_INSTALL_LIBDIR}/cmake/faspxx
        )
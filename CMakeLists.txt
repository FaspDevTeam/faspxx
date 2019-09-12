cmake_minimum_required(VERSION 3.14)

project(faspxx)

set(CMAKE_CXX_STANDARD 11)

include_directories(include/)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cxx)
add_library(faspxx ${SRC_FILES})

# Test error code class
add_executable(testErrorCode test/testErrorCode.cxx)

# Test vector operations
add_executable(testVector test/testVector.cxx)

# Test Google Test environment
# add_executable(testGoogle test/testGoogle.cxx)
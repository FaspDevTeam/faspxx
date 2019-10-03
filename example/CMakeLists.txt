cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

include_directories(../src/)

aux_source_directory(../src/ DIR_LIST)

add_executable(t ${SRC_LIST} ${DIR_LIST})

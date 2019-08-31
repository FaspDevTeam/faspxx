
cmake_minimum_required(VERSION 3.5)

project(FASPXX)

set (CMAKE_C_FLAGS_DEBUG "-D_DEBUG")
set (CMAKE_CXX_STANDARD 11)           # 以c++11标准编译
set (CMAKE_CXX_STANDARD_REQUIRED ON)  # 以c++11标准编译
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


add_subdirectory(tests bin)
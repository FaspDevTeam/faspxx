
cmake_minimum_required(VERSION 3.5)

project(learn_gtest)

set (CMAKE_C_FLAGS_DEBUG "-D_DEBUG")
set (CMAKE_CXX_STANDARD 11)           # 以c++11标准编译
set (CMAKE_CXX_STANDARD_REQUIRED ON)  # 以c++11标准编译
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


include_directories (
        "/home/fan/Downloads/googletest/googletest/include"
)

link_directories (
        "/home/fan/Downloads/googletest/googletest/build/lib"
)

link_libraries (
        "libgtest.a"
        "libgtest_main.a"
)

add_executable(gtest_examples gtest_examples.cpp)


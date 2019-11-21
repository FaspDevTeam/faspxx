cmake_minimum_required(VERSION 3.10)
project(project)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(p test.cxx)

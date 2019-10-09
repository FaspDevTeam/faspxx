cmake_minimum_required(VERSION 2.8)

set(SRC_LIST  VEC.old.cxx VECSpeedCompare.cxx)

include_directories(./)

add_executable(p ${SRC_LIST})
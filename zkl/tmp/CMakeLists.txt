cmake_minimum_required(VERSION 2.8)
PROJECT(demo)
set(SRC_LIST mat.cxx)

INCLUDE_DIRECTORIES(./)

ADD_EXECUTABLE(de ${SRC_LIST})
cmake_minimum_required(VERSION 3.10)
project(Tree)

set(CMAKE_CXX_STANDARD 17)
SET(GCC_COMPILE_FLAGS "-pthread")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

add_executable(Tree main.cpp BPlusTree.cpp BPlusTree.h struct.h allocator.cpp allocator.h lockManager.cpp lockManager.h)
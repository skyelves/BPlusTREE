cmake_minimum_required(VERSION 3.14)
project(Tree)

set(CMAKE_CXX_STANDARD 14)

add_executable(Tree main.cpp BPlusTree.cpp BPlusTree.h struct.h)
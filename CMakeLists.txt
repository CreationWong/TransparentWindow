cmake_minimum_required(VERSION 3.29)
project(TransparentWindow)

set(CMAKE_CXX_STANDARD 26)

add_library(TransparentWindow SHARED code/TransparentWindow.cpp)

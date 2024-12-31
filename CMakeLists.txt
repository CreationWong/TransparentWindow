cmake_minimum_required(VERSION 3.26)
project(TransparentWindow)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(TransparentWindow SHARED code/TransparentWindow.cpp)
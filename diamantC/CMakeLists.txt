cmake_minimum_required(VERSION 3.6)
project(diamantC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES diamond.c game.c diamond.h Node.c main.cpp)
add_executable(diamantC ${SOURCE_FILES})
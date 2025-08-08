cmake_minimum_required(VERSION 3.31)
project(LZ77)

set(CMAKE_CXX_STANDARD 20)


add_executable(LZ77 src/main.cpp
        src/LZ77.cpp
        include/LZ77.h)

include_directories(include)

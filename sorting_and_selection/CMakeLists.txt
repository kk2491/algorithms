project("sorting and selection")

cmake_minimum_required (VERSION 3.5)

add_definitions(-std=c++11)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

set(sources src/main.cpp
        src/selection.h
        src/sorting.h
        src/utilities.h
        src/my_unittest.h)

add_executable(run ${sources})
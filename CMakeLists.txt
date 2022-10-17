cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(2Q_cache)

add_executable(2Q.out src/main.cpp src/caches.hpp)

set (CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-std=c++20 ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

set_target_properties(
    2Q.out PROPERTIES
    CXX_STANDART 20
    CXX_STANDART_REQUIRED ON
)

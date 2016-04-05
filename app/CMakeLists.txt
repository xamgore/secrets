cmake_minimum_required(VERSION 3.3)
project(app)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp utils.h md5.h md5.cpp)
add_executable(app ${SOURCE_FILES})
cmake_minimum_required(VERSION 3.10)

project(Greedy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BOT_SIMPLE_REACT "use simple react" ON)

add_executable(${PROJECT_NAME} main.cpp)
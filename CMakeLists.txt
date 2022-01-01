cmake_minimum_required (VERSION 2.8.11)

project (slimey)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(${PROJECT_NAME} src/main.cpp)
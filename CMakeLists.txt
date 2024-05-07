cmake_minimum_required(VERSION 3.22)

project(slimey)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_subdirectory(lib/glfw)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(slimey src/main.cpp)

target_link_libraries(slimey glfw GLEW::GLEW OpenGL::GL)

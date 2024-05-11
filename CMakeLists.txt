cmake_minimum_required(VERSION 3.22)

project(slimey)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_subdirectory(lib/glfw)

add_compile_options(-Wall -Wextra -Wpedantic -std=c++20)

add_library(gl_wrapper INTERFACE)
target_include_directories(gl_wrapper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/gl_wrapper)
target_link_libraries(gl_wrapper INTERFACE GLEW::GLEW OpenGL::GL)

add_executable(slimey src/main.cpp)

target_link_libraries(slimey PRIVATE glfw GLEW::GLEW OpenGL::GL)
target_link_libraries(slimey PRIVATE gl_wrapper)

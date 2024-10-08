cmake_minimum_required(VERSION 3.22)

project(slimey)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)

add_compile_options(-Wall -Wextra -Wpedantic -std=c++20)

add_library(gl_wrapper INTERFACE)
target_include_directories(gl_wrapper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/gl_wrapper)
target_link_libraries(gl_wrapper INTERFACE GLEW::GLEW OpenGL::GL)

add_library(simulation INTERFACE)
target_include_directories(simulation INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/simulation)
target_link_libraries(simulation INTERFACE glm::glm)

add_executable(slimey src/main.cpp)

target_link_libraries(slimey PRIVATE glfw GLEW::GLEW OpenGL::GL glm::glm)
target_link_libraries(slimey PRIVATE gl_wrapper simulation)

add_custom_command(
  TARGET slimey POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/shaders
          ${CMAKE_BINARY_DIR}/shaders)

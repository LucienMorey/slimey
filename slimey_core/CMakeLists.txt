cmake_minimum_required (VERSION 2.8.11)

project(slimey_core)
set(VERSION_MAJOR "3")
set(VERSION_MINOR "16")
set(VERSION_PATCH "3")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set (SLIMEY_CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} src/main.cpp src/window.cpp src/slime_sim.cpp)

find_package(OpenGL REQUIRED)
# find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLAD_INCLUDE_DIR})
include_directories(${SLIMEY_CORE_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw glad)
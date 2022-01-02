cmake_minimum_required (VERSION 2.8.11)

project(glad)
set(VERSION_MAJOR "3")
set(VERSION_MINOR "16")
set(VERSION_PATCH "3")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set (GLAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (GLAD_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(GLAD_SRCS 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c
)

include_directories(${GLAD_SOURCE_DIR})
include_directories(${GLAD_INCLUDE_DIR})

add_library(glad STATIC ${GLAD_SRCS} ${GLAD_INCLUDE_DIR})

target_include_directories (glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
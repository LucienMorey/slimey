cmake_minimum_required (VERSION 2.8.11)

set(CMAKE_CXX_STANDARD 20)

project(slimey_core)
set(VERSION_MAJOR "3")
set(VERSION_MINOR "16")
set(VERSION_PATCH "3")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set (SLIMEY_CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/ui/window.cpp
        src/simulation/slime_sim.cpp
        # src/simulation/agent.cpp
        # src/simulation/PhysarumSimulator.cpp
        # src/simulation/generators/UniformParticleGenerator.cpp
        # src/simulation/generators/InwardCircleParticleGenerator.cpp
        # src/simulation/generators/PointParticleGenerator.cpp
        # src/simulation/generators/RandomCircleParticleGenerator.cpp
        # src/simulation/generators/RandomParticleGenerator.cpp
        src/rendering/SpriteL.cpp
        # src/rendering/physarum_renderer.cpp
        src/utils/files.cpp
        )


find_package(OpenGL REQUIRED)
# find_package(GLM REQUIRED)
# find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLAD_INCLUDE_DIR})
include_directories(${SLIMEY_CORE_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw glad glm::glm geGL)# magic_enum range-v3)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders" ${CMAKE_BINARY_DIR}/resources/shaders)

# install(DIRECTORY launch/
#   DESTINATION ${}/launch
#   PATTERN ".svn" EXCLUDE
# )
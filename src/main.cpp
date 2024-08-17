#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include <chrono>
#include <fstream>
#include <gl_wrapper/buffer.hpp>
#include <gl_wrapper/index_buffer.hpp>
#include <gl_wrapper/shader.hpp>
#include <gl_wrapper/shader_program.hpp>
#include <gl_wrapper/texture2d32f.hpp>
#include <gl_wrapper/vertex_array.hpp>
#include <gl_wrapper/vertex_buffer.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <random>
#include <simulation/agent.hpp>
#include <simulation/renderer.hpp>
#include <simulation/simulator.hpp>
#include <simulation/window.hpp>
#include <sstream>
#include <vector>

std::string read_text_from_file(std::string file_path)
{
  std::ifstream file(file_path);
  std::stringstream text;
  text << file.rdbuf();
  return text.str();
}

void print_error_and_exit(const std::string_view & message, int exit_code)
{
  std::cerr << message << std::endl;
  exit(exit_code);
}

// Window Context parameters
constexpr int32_t SCREEN_WIDTH = 640;
constexpr int32_t SCREEN_HEIGHT = 480;

// simulation parameters
constexpr uint32_t NUM_AGENTS = 5000;

// agent parameters
constexpr Slimey::AgentSettings agent_settings{
  .linear_speed = 30.0,
  .angular_speed = M_PI / 3.0,
  .sensor_radius = 1,
  .sensor_look_ahead = 35.0,
  .sensor_offset = M_PI / 3.0};

constexpr glm::vec4 SPECIES_MASK = {1.0, 1.0, 1.0, 1.0};

// trail parameters
constexpr Slimey::TrailSettings trail_settings = {
  .evaporation_rate = 0.3, .diffuse_weight = 0.08, .diffuse_radius = 1};

int main()
{
  Slimey::Window window;
  auto result = window.initialise(SCREEN_WIDTH, SCREEN_HEIGHT);
  if (result.first != 0) {
    print_error_and_exit(result.second, result.first);
  }

  // initialise glew
  if (glewInit() != GLEW_OK) {
    print_error_and_exit("cant init glew", -3);
  }

  Slimey::Simulator<SCREEN_WIDTH, SCREEN_HEIGHT, NUM_AGENTS> simulator;
  result = simulator.initialise(
    agent_settings, trail_settings, read_text_from_file("shaders/agent.glsl"),
    read_text_from_file("shaders/trail.glsl"));

  if (result.first != 0) {
    print_error_and_exit(result.second, result.first);
  }

  Slimey::Rendering::Renderer renderer;
  result = renderer.initialise(
    read_text_from_file("shaders/fragment_shader.glsl"),
    read_text_from_file("shaders/vertex_shader.glsl"), simulator.get_trail_map_base());

  if (result.first != 0) {
    print_error_and_exit(result.second, result.first);
  }

  auto last_time = std::chrono::steady_clock::now().time_since_epoch().count() / 1e9;

  /* Loop until the user closes the window */
  while (!window.should_close()) {
    auto current_time = std::chrono::steady_clock::now().time_since_epoch().count() / 1e9;
    auto last_step_length = (current_time - last_time);
    last_time = current_time;

    simulator.step(current_time, last_step_length);

    renderer.draw();

    window.update();
  }

  return 0;
}

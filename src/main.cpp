#include <GL/glew.h>

#include <chrono>
#include <fstream>
#include <glm/vec3.hpp>
#include <iostream>
#include <simulation/agent.hpp>
#include <simulation/renderer.hpp>
#include <simulation/simulator.hpp>
#include <simulation/window.hpp>
#include <sstream>
#include <vector>

using Slimey::Simulation::SpawnMode;

using Clock = std::chrono::steady_clock;
double time_now() { return Clock::now().time_since_epoch().count() / 1e9; }

std::string read_text_from_file(std::string file_path)
{
  std::ifstream file(file_path);
  std::stringstream text;
  text << file.rdbuf();
  return text.str();
}

void print_error_and_exit(int exit_code, const std::string_view & message)
{
  std::cerr << message << std::endl;
  exit(exit_code);
}

// Window Context parameters
constexpr int32_t SCREEN_WIDTH = 2560;
constexpr int32_t SCREEN_HEIGHT = 1440;

// simulation parameters
constexpr uint32_t NUM_AGENTS = 500000;
constexpr SpawnMode SPAWN_MODE = SpawnMode::CIRCULAR;

// agent parameters
constexpr Slimey::AgentSettings agent_settings{
  .linear_speed = 30.0,
  .angular_speed = M_PI / 3.0,
  .sensor_radius = 1,
  .sensor_look_ahead = 35.0,
  .sensor_offset = M_PI / 3.0};

// The colour and number of species is defined here. the colour mode is rgb. There cannot be more than 4 species
const std::vector<glm::vec3> SPECIES_COLOURS = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};

// trail parameters
constexpr Slimey::TrailSettings trail_settings = {
  .evaporation_rate = 0.3, .diffuse_weight = 0.08, .diffuse_radius = 1};

int main()
{
  if (SPECIES_COLOURS.size() > 4) {
    print_error_and_exit(-7, "The simulation doesnt support more than 4 species at once");
  }

  Slimey::Window window;
  auto result = window.initialise(SCREEN_WIDTH, SCREEN_HEIGHT);
  if (result.first != 0) {
    print_error_and_exit(result.first, result.second);
  }

  // initialise glew
  if (glewInit() != GLEW_OK) {
    print_error_and_exit(-3, "cant init glew");
  }

  Slimey::Simulation::Simulator<SCREEN_WIDTH, SCREEN_HEIGHT, NUM_AGENTS> simulator;
  result = simulator.initialise(
    agent_settings, trail_settings, SPAWN_MODE, SPECIES_COLOURS,
    read_text_from_file("shaders/agent.glsl"), read_text_from_file("shaders/trail.glsl"));

  if (result.first != 0) {
    print_error_and_exit(result.first, result.second);
  }

  Slimey::Rendering::Renderer renderer;
  result = renderer.initialise(
    read_text_from_file("shaders/fragment_shader.glsl"),
    read_text_from_file("shaders/vertex_shader.glsl"), simulator.get_trail_map_base());

  if (result.first != 0) {
    print_error_and_exit(result.first, result.second);
  }

  double last_time = time_now();

  /* Loop until the user closes the window */
  while (!window.should_close()) {
    double current_time = time_now();
    double last_step_length = current_time - last_time;
    last_time = current_time;

    simulator.step(current_time, last_step_length);

    renderer.draw();

    window.update();
  }

  return 0;
}

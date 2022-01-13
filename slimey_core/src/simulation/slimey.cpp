#include "simulation/slimey.hpp"

const size_t NUM_AGENTS = 1000000;

const PositionMode POS_MODE = PositionMode::CIRCLE;

const int NUM_SPECIES = 1;

Slimey::Slimey(int win_width, int win_height, int swapInterval, bool isFullscreen = false)
{
  window = std::make_shared<Window>(
    win_width, win_height, "Physarum Simulation", swapInterval, isFullscreen);

  std::filesystem::path shader_dir = "/home/lucien/git/slimey/slimey_core/resources/shaders/";

  // create simulator shader
  sim = std::make_shared<SlimeSim>(
    win_width, win_height, shader_dir, NUM_AGENTS, NUM_SPECIES, POS_MODE);

  // create rendering shader
  renderer = std::make_shared<SlimeRenderer>(win_width, win_height, shader_dir, NUM_AGENTS);
}

Slimey::~Slimey() {}

void Slimey::run()
{
  while (!window->windowShouldClose()) {
    window->use();
    float deltaTime = window->getDeltaTime();

    sim->simulate(deltaTime);

    renderer->render(deltaTime);

    window->unuse();
  }
}
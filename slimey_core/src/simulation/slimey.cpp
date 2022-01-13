#include "simulation/slimey.hpp"

const size_t NUM_AGENTS = 1000000;

const PositionMode POS_MODE = PositionMode::CIRCLE;

const int NUM_SPECIES = 1;

const std::filesystem::path shader_dir = "/home/lucien/git/slimey/slimey_core/resources/shaders/";

Slimey::Slimey(int win_width, int win_height, int swapInterval, bool isFullscreen = false)
{
  // create slime window
  window = std::make_shared<Window>(
    win_width, win_height, "Physarum Simulation", swapInterval, isFullscreen);

  // create simulator shader
  sim = std::make_shared<SlimeSim>(
    win_width, win_height, shader_dir, NUM_AGENTS, NUM_SPECIES, POS_MODE);

  // create rendering shader
  renderer = std::make_shared<SlimeRenderer>(win_width, win_height, shader_dir, NUM_AGENTS);
}

Slimey::~Slimey() {}

void Slimey::run()
{
  // simulate until window is told to close
  while (!window->windowShouldClose())
  {
    window->use();
    // get time between frames
    float deltaTime = window->getDeltaTime();

    // sim slime movement over delta time
    sim->simulate(deltaTime);

    // render slime agents
    renderer->render(deltaTime);

    window->unuse();
  }
}
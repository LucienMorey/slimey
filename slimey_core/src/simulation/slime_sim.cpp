

#include "simulation/slime_sim.hpp"

SlimeSim::SlimeSim()
{
  window = std::make_unique<Window>(640, 480, "Physarum Simulation", 0, false);
}

SlimeSim::~SlimeSim(){

}

void SlimeSim::run()
{
  while (!window->windowShouldClose()) {
    window->use();

    glClearColor(0.2f, 0.2f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    window->unuse();
  }
}
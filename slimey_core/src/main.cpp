#include <iostream>
#include <memory>

#include "window.hpp"

int main()
{
  std::unique_ptr<Window> window;
  window = std::make_unique<Window>(640, 480, "Physarum Simulation", 0, false);

  while (!window->windowShouldClose()) {
    window->use();

    glClearColor(0.2f, 0.2f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    window->unuse();
  }

  return 0;
}
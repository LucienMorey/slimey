#ifndef SLIMEY_H
#define SLIMEY_H

#include <memory>

#include "ui/window.hpp"
#include "rendering/slime_renderer.hpp"
#include "simulation/slime_sim.hpp"

class Slimey{
  public:
    Slimey(int win_width, int win_height, int swapInterval, bool isFullscreen);
    ~Slimey();
    void run();

  private:
    std::shared_ptr<Window> window;

    std::shared_ptr<SlimeRenderer> renderer;
    std::shared_ptr<SlimeSim> sim;
};

#endif
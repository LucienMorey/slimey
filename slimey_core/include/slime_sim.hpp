#ifndef SLIME_SIM_H
#define SLIME_SIM_H

#include <memory>

#include "window.hpp"

class SlimeSim
{
public:
  SlimeSim();
  ~SlimeSim();
  void run();

private:
  std::unique_ptr<Window> window;
};

#endif
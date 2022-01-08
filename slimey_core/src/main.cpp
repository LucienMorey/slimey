#include <iostream>
#include <memory>
#include <unistd.h>
#include <stdio.h>
#include "simulation/slime_sim.hpp"

int main()
{
  std::unique_ptr<SlimeSim> sim = std::make_unique<SlimeSim>(1920, 1080, 0, false);
  sim->run();

  return 0;
}
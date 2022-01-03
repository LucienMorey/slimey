#include <iostream>
#include <memory>

#include "simulation/slime_sim.hpp"

int main()
{
  std::unique_ptr<SlimeSim> sim = std::make_unique<SlimeSim>();
  sim->run();

  return 0;
}
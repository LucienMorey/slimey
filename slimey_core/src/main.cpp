#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <memory>

#include "simulation/slimey.hpp"

int main()
{
  std::unique_ptr<Slimey> sim = std::make_unique<Slimey>(1920, 1080, 0, false);
  sim->run();

  return 0;
}
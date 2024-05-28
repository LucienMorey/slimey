#pragma once

#include <cstdint>
#include <glm/vec2.hpp>

namespace Slimey
{

struct Agent
{
  glm::vec2 position;
  float angle;
  float padding_;
};
};  // namespace Slimey

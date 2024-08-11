#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace Slimey
{

struct Agent
{
  glm::vec2 position;
  float angle;
  float padding_;
  glm::vec4 species_mask;
  float linear_speed;
  float angular_speed;
  int32_t sensor_radius;
  float sensor_look_ahead;
};
};  // namespace Slimey

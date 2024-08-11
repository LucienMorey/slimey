#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace Slimey
{

// BEWARE ALIGNMENT ENFORCED ON THIS STRUCT
struct Agent
{
  glm::vec4 species_mask;

  glm::vec2 position;
  float angle;
  float linear_speed;

  float angular_speed;
  int32_t sensor_radius;
  float sensor_look_ahead;
  float sensor_offset;
};
};  // namespace Slimey

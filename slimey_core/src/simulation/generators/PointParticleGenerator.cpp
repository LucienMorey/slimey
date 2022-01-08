//
// Created by xflajs00 on 01.11.2021.
//

#include "simulation/generators/PointParticleGenerator.h"

#include <utils/rand.h>

#include <numbers>

namespace physarum
{
PointParticleGenerator::PointParticleGenerator(glm::uvec2 point) : point(point) {}

std::vector<Agent> PointParticleGenerator::generateParticles(
  std::size_t count, std::uint32_t speciesID)
{
  auto result = std::vector<Agent>{};
  for (std::size_t i = 0; i < count; ++i) {
    result.emplace_back(
      point[0], point[1], fastRandom(0.f, 1.f) * std::numbers::pi_v<float> * 2.f, speciesID);
  }
  return result;
}
}  // namespace physarum
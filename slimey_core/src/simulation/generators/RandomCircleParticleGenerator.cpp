//
// Created by xflajs00 on 02.11.2021.
//

#include "simulation/generators/RandomCircleParticleGenerator.h"

#include <utils/rand.h>

#include <glm/common.hpp>

namespace physarum
{
RandomCircleParticleGenerator::RandomCircleParticleGenerator(const glm::ivec2 & size) : size(size)
{
}

std::vector<Agent> RandomCircleParticleGenerator::generateParticles(
  std::size_t count, std::uint32_t speciesID)
{
  auto result = std::vector<Agent>{};
  const auto center = glm::vec2{size / 2};
  const auto radius = glm::min(static_cast<float>(size.x), static_cast<float>(size.y)) * 0.4f;
  for (std::size_t i = 0; i < count; ++i) {
    const auto pos = center + fastRandomUnitCircle() * radius;
    const auto angle = fastRandom(0.f, 1.f) * std::numbers::pi_v<float> * 2.f;
    result.emplace_back(pos[0], pos[1], angle, speciesID);
  }
  return result;
}

}  // namespace physarum
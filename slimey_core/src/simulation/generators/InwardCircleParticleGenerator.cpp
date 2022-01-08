//
// Created by xflajs00 on 02.11.2021.
//

#include "simulation/generators/InwardCircleParticleGenerator.h"

#include <utils/rand.h>

#include <cmath>
#include <glm/common.hpp>
#include <glm/geometric.hpp>

namespace physarum
{
InwardCircleParticleGenerator::InwardCircleParticleGenerator(const glm::ivec2 & size) : size(size)
{
}

std::vector<Agent> physarum::InwardCircleParticleGenerator::generateParticles(
  std::size_t count, std::uint32_t speciesID)
{
  auto result = std::vector<Agent>{};
  const auto center = glm::vec2{size / 2};
  const auto radius = glm::min(static_cast<float>(size.x), static_cast<float>(size.y)) * 0.4f;
  for (std::size_t i = 0; i < count; ++i) {
    const auto pos = center + fastRandomUnitCircle() * radius;
    const auto angle = std::atan2(glm::normalize(center - pos).y, glm::normalize(center - pos).x);

    result.emplace_back(pos[0], pos[1], angle, speciesID);
  }
  return result;
}

}  // namespace physarum
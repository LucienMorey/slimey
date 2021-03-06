//
// Created by xflajs00 on 07.11.2021.
//

#include "simulation/generators/UniformParticleGenerator.h"

#include <utils/rand.h>

#include <cmath>

namespace physarum
{
UniformParticleGenerator::UniformParticleGenerator(glm::uvec2 imageSize, float step)
: size(imageSize), step(step)
{
}

std::vector<Agent> UniformParticleGenerator::generateParticles(
  std::size_t count, std::uint32_t speciesID)
{
  auto result = std::vector<Agent>{};
  for (std::size_t i = 0; i < count; ++i) {
    const auto rndAngle = fastRandom(0.f, 1.f) * std::numbers::pi_v<float> * 2.f;
    const auto xI = (i % size.x);
    const auto yI = (i / size.x) % size.y;
    const auto x = std::fmod(static_cast<float>(xI) * step, size.x);
    const auto y = std::fmod(static_cast<float>(yI) * step, size.y);
    result.emplace_back(x, y, rndAngle, speciesID);
  }
  return result;
}
}  // namespace physarum
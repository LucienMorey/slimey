//
// Created by xflajs00 on 01.11.2021.
//

#include "simulation/generators/RandomParticleGenerator.h"

#include <utils/rand.h>

#include <numbers>

namespace physarum
{
RandomParticleGenerator::RandomParticleGenerator(glm::uvec2 imageSize) : size(imageSize) {}

std::vector<Agent> RandomParticleGenerator::generateParticles(
  std::size_t count, std::uint32_t speciesID)
{
  auto result = std::vector<Agent>{};
  for (std::size_t i = 0; i < count; ++i) {
    const auto rndAngle = fastRandom(0.f, 1.f) * std::numbers::pi_v<float> * 2.f;
    result.emplace_back(
      fastRandom(0.f, static_cast<float>(size.x)), fastRandom(0.f, static_cast<float>(size.y)),
      rndAngle, speciesID);
  }
  return result;
}

}  // namespace physarum

#pragma once
#include <algorithm>
#include <chrono>
#include <glm/vec4.hpp>
#include <iostream>
#include <random>
#include <vector>

constexpr float TWO_PI = 6.2831853f;

struct Agent
{
  float x, y;
  float angle;
  int32_t speciesMask;
};

enum class PositionMode
{
  RANDOM,
  CIRCLE,
  CENTER,
  INWARD_CIRCLE
};

class AgentSystem
{
public:
  AgentSystem() : m_numAgents(0) {}
  AgentSystem(
    int width, int height, size_t numAgents, int8_t numSpecies,
    PositionMode posMode = PositionMode::RANDOM)
  : m_numAgents(numAgents)
  {
    if (numSpecies > 3)
      numSpecies = 3;
    else if (numSpecies < -1)
      numSpecies = -1;

    std::default_random_engine randGen(std::chrono::duration_cast<std::chrono::nanoseconds>(
                                         std::chrono::system_clock::now().time_since_epoch())
                                         .count());
    // std::uniform_real_distribution<float> position_x(0.0f, (float)width);
    // std::uniform_real_distribution<float> position_y(0.0f, (float)height);
    std::uniform_real_distribution<float> angleDistribution(0.0f, TWO_PI);
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    std::uniform_int_distribution<int32_t> species(0, numSpecies - 1);

    int centre_x = width / 2;
    int centre_y = height / 2;

    m_agents.resize(m_numAgents);

    for (int i = 0; i < m_agents.size(); i++)
    {
      float pos_x, pos_y;
      float ang = angleDistribution(randGen);
      switch (posMode)
      {
        case (PositionMode::CIRCLE): {
          float rad = distribution(randGen) * height * 0.3;
          if (rad > 540)
          {
            std::cout << rad << std::endl;
          }
          float circle_ang = angleDistribution(randGen);
          pos_x = centre_x + rad * cos(circle_ang);
          pos_y = centre_y + rad * sin(circle_ang);
          break;
        }
        case (PositionMode::CENTER): {
          pos_x = centre_x;
          pos_y = centre_y;
          break;
        }
        case (PositionMode::RANDOM): {
          pos_x = distribution(randGen) * width;
          pos_y = distribution(randGen) * height;
        }
        case (PositionMode::INWARD_CIRCLE): {
          float rad = distribution(randGen) * height * 0.3;
          float circle_ang = angleDistribution(randGen);
          pos_y = centre_y + rad * sin(circle_ang);
          pos_x = centre_x + rad * cos(circle_ang);
          ang = atan2(pos_y - centre_y, pos_x - centre_x);
        }
      }

      m_agents.at(i) = {pos_x, pos_y, ang, (numSpecies < 2) ? -1 : species(randGen)};
    }
  }

  const std::vector<Agent> & getAgents() const { return m_agents; }

private:
  size_t m_numAgents;
  std::vector<Agent> m_agents;
};
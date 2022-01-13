

#include <utils/files.h>

#include <chrono>
#include <vector>

#include "simulation/agent.hpp"
#include "simulation/slime_sim.hpp"

SlimeSim::SlimeSim(
  int win_width, int win_height, std::filesystem::path shader_dir, size_t num_agents,
  int num_species, PositionMode pos_mode)
: num_agents_(num_agents)
{
  // generate agents
  agent_generator =
    std::make_shared<AgentSystem>(win_width, win_height, num_agents_, num_species, pos_mode);

  // create simulation compute shader
  const auto agentShaderSrc = physarum::readFile(shader_dir / "agent.comp.glsl");
  if (!agentShaderSrc.has_value())
  {
    throw std::runtime_error("Could not load 'agent.comp.glsl'");
  }
  agentShader = std::make_shared<Shader>(GL_COMPUTE_SHADER, agentShaderSrc.value());
  agentComputeProgram = std::make_shared<Program>(agentShader);

  // bind  agent buffer to the simulation
  agentBuffer = std::make_shared<Buffer>(num_agents_ * sizeof(Agent), nullptr, GL_DYNAMIC_DRAW);
  agentBuffer->setData(agent_generator->getAgents());
}

SlimeSim::~SlimeSim() {}

void SlimeSim::init() {}

void SlimeSim::simulate(float delta_time)
{
  //update delta time and dispatch next iteration of thread
  agentComputeProgram->use();
  agentComputeProgram->set1f("deltaTime", delta_time);
  agentBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 2);
  agentComputeProgram->dispatch(num_agents_ / 64 + 1, 1, 1);

  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
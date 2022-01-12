

#include <utils/files.h>

#include <chrono>
#include <vector>

#include "simulation/agent.hpp"
#include "simulation/slime_sim.hpp"

const size_t NUM_AGENTS = 1000000;

SlimeSim::SlimeSim(int win_width, int win_height, int swapInterval, bool isFullscreen)
{
  agent_generator =
    std::make_shared<AgentSystem>(win_width, win_height, NUM_AGENTS, 1, PositionMode::CIRCLE);

  window = std::make_shared<Window>(
    win_width, win_height, "Physarum Simulation", swapInterval, isFullscreen);

  std::filesystem::path shader_dir = "/home/lucien/git/slimey/slimey_core/resources/shaders/";

  const auto agentShaderSrc = physarum::readFile(shader_dir / "agent.comp.glsl");
  if (!agentShaderSrc.has_value()) {
    throw std::runtime_error("Could not load 'physarum_sim.comp'");
  }
  agentShader = std::make_shared<Shader>(GL_COMPUTE_SHADER, agentShaderSrc.value());
  agentComputeProgram = std::make_shared<Program>(agentShader);

  agentBuffer = std::make_shared<Buffer>(NUM_AGENTS * sizeof(Agent), nullptr, GL_DYNAMIC_DRAW);
  agentBuffer->setData(agent_generator->getAgents());

  renderer = std::make_shared<SlimeRenderer>(win_width, win_height, shader_dir, NUM_AGENTS);
}

SlimeSim::~SlimeSim() {}

void SlimeSim::run()
{
  while (!window->windowShouldClose()) {
    window->use();
    float deltaTime, currentTime;
    window->getDeltaTime(currentTime, deltaTime);

    agentComputeProgram->use();
    agentComputeProgram->set1f("deltaTime", deltaTime);
    agentBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 2);
    agentComputeProgram->dispatch(NUM_AGENTS / 64 + 1, 1, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

    renderer->render(deltaTime);

    window->unuse();
  }
}
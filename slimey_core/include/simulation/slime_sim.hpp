#ifndef SLIME_SIM_H
#define SLIME_SIM_H

#include <geGL/Buffer.h>
#include <geGL/Program.h>
#include <geGL/Shader.h>

#include <filesystem>
#include <memory>

#include "simulation/agent.hpp"

class SlimeSim
{
public:
  SlimeSim(
    int win_width, int win_height, std::filesystem::path shader_dir, size_t num_agents,
    int num_species, PositionMode pos_mode);
  ~SlimeSim();
  void init();
  void simulate(float delta_time);

private:
  size_t num_agents_;

  std::shared_ptr<AgentSystem> agent_generator;

  std::shared_ptr<Program> agentComputeProgram;
  std::shared_ptr<Shader> agentShader;
  std::shared_ptr<Buffer> agentBuffer;
};

#endif
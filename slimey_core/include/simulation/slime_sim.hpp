#ifndef SLIME_SIM_H
#define SLIME_SIM_H

#include <geGL/Texture.h>
#include <geGL/Shader.h>
#include <geGL/Program.h>
#include <geGL/Buffer.h>

#include <memory>


#include "rendering/SpriteL.hpp"
#include "simulation/agent.hpp"
#include "ui/window.hpp"

class SlimeSim
{
public:
  SlimeSim(int win_width, int win_height, int swapInterval, bool isFullscreen = false);
  ~SlimeSim();
  void run();

private:

  std::shared_ptr<Window> window;
  std::shared_ptr<AgentSystem> agent_generator;

  std::shared_ptr<SpriteL> m_quad;

  std::shared_ptr<Program> agentComputeProgram;
  std::shared_ptr<Program> textureComputeProgram;
  std::shared_ptr<Program> renderQuadProgram;

  std::shared_ptr<Shader> agentShader;
  std::shared_ptr<Shader> textureShader;
  std::shared_ptr<Buffer> agentBuffer;

  std::shared_ptr<Texture> initialTexture;
  std::shared_ptr<Texture> processedTexture;
};

#endif
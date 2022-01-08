#ifndef SLIME_SIM_H
#define SLIME_SIM_H

#include <geGL/Texture.h>

#include <memory>

#include "rendering/ComputeShaderL.hpp"
#include "rendering/ShaderL.hpp"
#include "rendering/SpriteL.hpp"
#include "rendering/TextureL.hpp"
#include "simulation/agent.hpp"
#include "ui/window.hpp"

class SlimeSim
{
public:
  SlimeSim(int win_width, int win_height, int swapInterval, bool isFullscreen = false);
  ~SlimeSim();
  void run();

private:
  // AgentSystem m_agentSystem;

  std::unique_ptr<Window> window;
  std::unique_ptr<AgentSystem> agent_generator;

  std::unique_ptr<ShaderL> m_program;
  std::unique_ptr<ComputeShaderL> m_agentComputeProgram;
  std::unique_ptr<ComputeShaderL> m_textureComputeProgram;

  std::unique_ptr<TextureL> m_initialTexture;
  std::unique_ptr<TextureL> m_processedTexture;
  std::unique_ptr<SpriteL> m_quad;

  std::unique_ptr<Texture> initialTexture;
  std::unique_ptr<Texture> processedTexture;

  // std::shared_ptr<Buffer> particleBuffer;
  // std::shared_ptr<ShaderL> simulateShader;
  // std::shared_ptr<Program> simulateProgram;

  // std::shared_ptr<Texture> renderTexture;
  // std::shared_ptr<Program> renderTextureProgram;
  // std::shared_ptr<Program> renderQuadProgram;
  // std::unique_ptr<physarum::PhysarumRenderer> renderer;
};

#endif
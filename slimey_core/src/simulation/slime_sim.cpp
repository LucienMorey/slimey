

#include "simulation/slime_sim.hpp"
#include "simulation/agent.hpp"

#include <utils/files.h>

#include <chrono>
#include <vector>

const size_t NUM_AGENTS = 1000000;

SlimeSim::SlimeSim(int win_width, int win_height, int swapInterval, bool isFullscreen)
{
  agent_generator =
    std::make_shared<AgentSystem>(win_width, win_height, NUM_AGENTS, 1, PositionMode::CIRCLE);

  window = std::make_shared<Window>(
    win_width, win_height, "Physarum Simulation", swapInterval, isFullscreen);

  std::filesystem::path shader_dir = "/home/lucien/git/slimey/slimey_core/resources/shaders/";

  const auto renderVertSrc = readFile(shaderDir / "main.vert.glsl");
  if (!renderVertSrc.has_value()) { throw std::runtime_error("Could not load 'render.vert'"); }
  auto renderVertShader = std::make_shared<Shader>(GL_VERTEX_SHADER, renderVertSrc.value());
  const auto renderFragSrc = readFile(shaderDir / "main.frag.glsl");
  if (!renderFragSrc.has_value()) { throw std::runtime_error("Could not load 'render.frag'"); }
  auto renderFragShader = std::make_shared<Shader>(GL_FRAGMENT_SHADER, renderFragSrc.value());
  renderQuadProgram = std::make_shared<Program>(renderVertShader, renderFragShader);

  m_program = std::make_shared<ShaderL>(
    "/home/lucien/git/slimey/slimey_core/resources/shaders/main.vert.glsl",
    "/home/lucien/git/slimey/slimey_core/resources/shaders/main.frag.glsl");
  // m_agentComputeProgram = std::make_shared<ComputeShaderL>(
  //   "/home/lucien/git/slimey/slimey_core/resources/shaders/agent.comp.glsl");
  // m_textureComputeProgram = std::make_shared<ComputeShaderL>(
  //   "/home/lucien/git/slimey/slimey_core/resources/shaders/textureProc.comp.glsl");

  const auto agentShaderSrc = physarum::readFile(shader_dir / "agent.comp.glsl");
  if (!agentShaderSrc.has_value()) { throw std::runtime_error("Could not load 'physarum_sim.comp'"); }
  agentShader = std::make_shared<Shader>(GL_COMPUTE_SHADER, agentShaderSrc.value());
  agentComputeProgram = std::make_shared<Program>(agentShader);

  agentBuffer = std::make_shared<Buffer>(NUM_AGENTS * sizeof(Agent), nullptr, GL_DYNAMIC_DRAW);
  agentBuffer->setData(agent_generator->getAgents());

  const auto textureShaderSrc = physarum::readFile(shader_dir / "textureProc.comp.glsl");
  if (!textureShaderSrc.has_value()) { throw std::runtime_error("Could not load 'physarum_sim.comp'"); }
  textureShader = std::make_shared<Shader>(GL_COMPUTE_SHADER, textureShaderSrc.value());
  textureComputeProgram = std::make_shared<Program>(textureShader);

  // m_agentComputeProgram->useShaderStorageBuffer(
  //   NUM_AGENTS * sizeof(Agent), (void *)&agent_generator->getAgents()[0]);

  initialTexture = std::make_shared<Texture>(GL_TEXTURE_2D, GL_RGBA32F, 1, win_width, win_height);
  initialTexture->bindImage(0, 0, GL_RGBA32F, GL_READ_WRITE, GL_FALSE, 0);

  processedTexture = std::make_shared<Texture>(GL_TEXTURE_2D, GL_RGBA32F, 1, win_width, win_height);
  processedTexture->bindImage(0, 0, GL_RGBA32F, GL_WRITE_ONLY, GL_FALSE, 0);

  m_quad = std::make_shared<SpriteL>(-1.0f, 1.0f, 1.0f, -1.0f, processedTexture->getId());
}

SlimeSim::~SlimeSim() {}

void SlimeSim::run()
{
  GLuint groups_a = NUM_AGENTS / 64;
  GLuint groups_x = window->getWidth() / 8;
  GLuint groups_y = window->getHeight() / 8;

  while (!window->windowShouldClose()) {
    window->use();
    float deltaTime, currentTime;
    window->getDeltaTime(currentTime, deltaTime);

    agentComputeProgram->use();
    agentComputeProgram->set1f("deltaTime", deltaTime);
    agentBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 2);
    agentComputeProgram->dispatch(NUM_AGENTS/64 +1, 1, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

    textureComputeProgram->use();
    textureComputeProgram->set1f("deltaTime", deltaTime);
    textureComputeProgram->dispatch(groups_x, groups_y,1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->use();
    m_quad->draw();

    window->unuse();
  }
}
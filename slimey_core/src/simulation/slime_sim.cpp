

#include "simulation/slime_sim.hpp"

#include <utils/files.h>

#include <chrono>
#include <vector>

const size_t NUM_AGENTS = 2000000;

SlimeSim::SlimeSim(int win_width, int win_height, int swapInterval, bool isFullscreen)
{
  agent_generator =
    std::make_unique<AgentSystem>(win_width, win_height, NUM_AGENTS, 1, PositionMode::CIRCLE);

  window = std::make_unique<Window>(
    win_width, win_height, "Physarum Simulation", swapInterval, isFullscreen);

  std::filesystem::path shader_dir = "/home/lucien/git/slimey/slimey_core/resources/shaders/";

  m_program = std::make_unique<ShaderL>(
    "/home/lucien/git/slimey/slimey_core/resources/shaders/main.vert.glsl",
    "/home/lucien/git/slimey/slimey_core/resources/shaders/main.frag.glsl");
  m_agentComputeProgram = std::make_unique<ComputeShaderL>(
    "/home/lucien/git/slimey/slimey_core/resources/shaders/agent.comp.glsl");
  m_textureComputeProgram = std::make_unique<ComputeShaderL>(
    "/home/lucien/git/slimey/slimey_core/resources/shaders/textureProc.comp.glsl");

  m_agentComputeProgram->useShaderStorageBuffer(
    NUM_AGENTS * sizeof(Agent), (void *)&agent_generator->getAgents()[0]);

  initialTexture = std::make_unique<Texture>(GL_TEXTURE_2D, GL_RGBA32F, 1, win_width, win_height);
  initialTexture->bindImage(0, 0, GL_RGBA32F, GL_READ_WRITE, GL_FALSE, 0);

  processedTexture = std::make_unique<Texture>(GL_TEXTURE_2D, GL_RGBA32F, 1, win_width, win_height);
  processedTexture->bindImage(0, 0, GL_RGBA32F, GL_WRITE_ONLY, GL_FALSE, 0);

  m_quad = std::make_unique<SpriteL>(-1.0f, 1.0f, 1.0f, -1.0f, processedTexture->getId());
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

    m_agentComputeProgram->use();
    m_agentComputeProgram->setFloat("deltaTime", deltaTime);
    m_agentComputeProgram->dispatch(groups_a, 1, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

    m_textureComputeProgram->use();
    m_textureComputeProgram->setFloat("deltaTime", deltaTime);
    m_textureComputeProgram->dispatch(groups_x, groups_y, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->use();
    m_quad->draw();

    window->unuse();
  }
}
#include <utils/files.h>

#include "rendering/slime_renderer.hpp"

SlimeRenderer::SlimeRenderer(
  int width, int height, std::filesystem::path shader_dir, size_t num_agents)
: num_agents_(num_agents), width_(width), height_(height), shader_dir_(shader_dir)
{
  const auto renderVertSrc = physarum::readFile(shader_dir_ / "main.vert.glsl");
  if (!renderVertSrc.has_value()) {
    throw std::runtime_error("Could not load 'render.vert'");
  }
  auto renderVertShader = std::make_shared<Shader>(GL_VERTEX_SHADER, renderVertSrc.value());
  const auto renderFragSrc = physarum::readFile(shader_dir_ / "main.frag.glsl");
  if (!renderFragSrc.has_value()) {
    throw std::runtime_error("Could not load 'render.frag'");
  }
  auto renderFragShader = std::make_shared<Shader>(GL_FRAGMENT_SHADER, renderFragSrc.value());
  renderQuadProgram = std::make_shared<Program>(renderVertShader, renderFragShader);

  const auto textureShaderSrc = physarum::readFile(shader_dir_ / "textureProc.comp.glsl");
  if (!textureShaderSrc.has_value()) {
    throw std::runtime_error("Could not load 'physarum_sim.comp'");
  }
  textureShader = std::make_shared<Shader>(GL_COMPUTE_SHADER, textureShaderSrc.value());
  textureComputeProgram = std::make_shared<Program>(textureShader);

  initialTexture = std::make_shared<Texture>(GL_TEXTURE_2D, GL_RGBA32F, 1, width_, height_);
  initialTexture->bindImage(0, 0, GL_RGBA32F, GL_READ_WRITE, GL_FALSE, 0);

  processedTexture = std::make_shared<Texture>(GL_TEXTURE_2D, GL_RGBA32F, 1, width_, height_);
  processedTexture->bindImage(0, 0, GL_RGBA32F, GL_WRITE_ONLY, GL_FALSE, 0);
  processedTexture->texParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  processedTexture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  const std::vector<float> quadVertices{
      -1.0f, 1.0f, 0.0f,
      0.0f, 0.0f,
      -1.0f, -1.0f, 0.0f,
      0.0f, 1.0f,
      1.0f,  1.0f, 0.0f,
      1.0f, 0.0f,
      1.0f,  -1.0f, 0.0f,
      1.0f, 1.0f,
  };
  quadVBO = std::make_shared<Buffer>(sizeof(float) * quadVertices.size(), quadVertices.data());
  quadVAO = std::make_shared<VertexArray>();
  quadVAO->addAttrib(quadVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), 0);
  quadVAO->addAttrib(quadVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (3 * sizeof(float)));
}

SlimeRenderer::~SlimeRenderer() {}

void SlimeRenderer::init() {}

void SlimeRenderer::render(float delta_time)
{

  textureComputeProgram->use();
  textureComputeProgram->set1f("deltaTime", delta_time);
  textureComputeProgram->dispatch(width_ / 8, height_ / 8);

  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  renderQuadProgram->use();
  quadVAO->bind();
  processedTexture->bind(0);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

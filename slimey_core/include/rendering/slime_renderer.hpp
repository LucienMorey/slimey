#ifndef SLIME_RENDERER_H
#define SLIME_RENDERER_H

#include <filesystem>

#include <geGL/Buffer.h>
#include <geGL/Program.h>
#include <geGL/Shader.h>
#include <geGL/Texture.h>

#include "rendering/SpriteL.hpp"

class SlimeRenderer
{
public:
  SlimeRenderer(int width, int height,std::filesystem::path shader_dir, size_t num_agents);
  ~SlimeRenderer();

  void init();

  void render(float delta_time);

private:
  size_t num_agents_;
  int width_;
  int height_;
  std::filesystem::path shader_dir_;

  std::shared_ptr<SpriteL> m_quad;

  std::shared_ptr<Program> textureComputeProgram;
  std::shared_ptr<Program> renderQuadProgram;

  std::shared_ptr<Shader> textureShader;

  std::shared_ptr<Texture> initialTexture;
  std::shared_ptr<Texture> processedTexture;
};

#endif
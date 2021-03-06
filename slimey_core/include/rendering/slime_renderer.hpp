#ifndef SLIME_RENDERER_H
#define SLIME_RENDERER_H

#include <geGL/Buffer.h>
#include <geGL/Program.h>
#include <geGL/Shader.h>
#include <geGL/Texture.h>
#include <geGL/VertexArray.h>

#include <filesystem>

class SlimeRenderer
{
public:
  SlimeRenderer(int width, int height, std::filesystem::path shader_dir, size_t num_agents);
  ~SlimeRenderer();

  void init();

  void render(float delta_time);

private:
  size_t num_agents_;
  int width_;
  int height_;
  std::filesystem::path shader_dir_;

  std::shared_ptr<Program> textureComputeProgram;
  std::shared_ptr<Program> renderQuadProgram;

  std::shared_ptr<Shader> textureShader;

  std::shared_ptr<Texture> initialTexture;
  std::shared_ptr<Texture> processedTexture;

  std::shared_ptr<Buffer> quadVBO;
  std::shared_ptr<VertexArray> quadVAO;
};

#endif
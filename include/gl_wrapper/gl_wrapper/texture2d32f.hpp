#pragma once

#include <GL/glew.h>

#include <glm/vec4.hpp>
#include <vector>
namespace GlWrapper
{

template <size_t width, size_t height, uint32_t base>
class Texture2d32f
{
public:
  Texture2d32f()
  {
    // check lib types match inbuilt types
    static_assert(sizeof(uint32_t) == sizeof(GLuint), "GLuint is expected to be 32bits in size");
    static_assert(base < 32, "binding base needs to be less than 32");
    base_ = base;
    glGenTextures(1, &id_);
    bind();
    data_.resize(width * height * 4);
    std::fill(data_.begin(), data_.end(), glm::vec4(0.0, 0.0, 0.0, 0.0));
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, data_.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindImageTexture(0, id_, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
  }

  ~Texture2d32f() { glDeleteTextures(1, &id_); }

  void bind() const
  {
    glActiveTexture(GL_TEXTURE0 + base_);
    glBindTexture(GL_TEXTURE_2D, id_);
  }
  void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

  uint32_t get_base_id() const { return base_; }

private:
  uint32_t id_;
  uint32_t base_;
  std::vector<glm::vec4> data_;
};

};  // namespace GlWrapper

#pragma once

#include <GL/glew.h>

#include <array>
#include <span>

namespace GlWrapper
{

class IndexBuffer
{
public:
  IndexBuffer() = delete;

  IndexBuffer(const std::span<std::array<uint32_t, 3>> buffer)
  {
    // check lib types match inbuilt types
    static_assert(sizeof(uint32_t) == sizeof(GLuint), "GLuint is expected to be 32bits in size");

    glGenBuffers(1, &id_);
    bind();
    glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, buffer.size() * 3 * sizeof(uint32_t), buffer.data(), GL_STATIC_DRAW);
  }

  ~IndexBuffer() { glDeleteBuffers(1, &id_); }
  void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_); }
  void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

private:
  uint32_t id_;
};

};  // namespace GlWrapper

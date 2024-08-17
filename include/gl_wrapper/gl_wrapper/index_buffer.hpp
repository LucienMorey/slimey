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
    vertex_count_ = buffer.size() * 3;
    glGenBuffers(1, &id_);
    bind();
    glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, vertex_count_ * sizeof(uint32_t), buffer.data(), GL_STATIC_DRAW);
  }

  ~IndexBuffer() { glDeleteBuffers(1, &id_); }
  void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_); }
  void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
  uint32_t get_vertex_count() const { return vertex_count_; }

private:
  uint32_t id_;
  uint32_t vertex_count_;
};

};  // namespace GlWrapper

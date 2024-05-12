#pragma once

#include <GL/glew.h>

#include <gl_wrapper/vertex_buffer_layout.hpp>
#include <span>

namespace GlWrapper
{

template <class T>
class VertexBuffer
{
public:
  VertexBuffer() = delete;

  VertexBuffer(const std::span<T> buffer)
  {
    // check lib types match inbuilt types
    static_assert(sizeof(uint32_t) == sizeof(GLuint), "GLuint is expected to be 32bits in size");

    glGenBuffers(1, &id_);
    bind();
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(T), buffer.data(), GL_STATIC_DRAW);
  }

  ~VertexBuffer() { glDeleteBuffers(1, &id_); }
  void bind() const { glBindBuffer(GL_ARRAY_BUFFER, id_); }
  void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

  VertexBufferLayout & get_layout() { return layout_; }

private:
  VertexBufferLayout layout_;
  uint32_t id_;
};

};  // namespace GlWrapper

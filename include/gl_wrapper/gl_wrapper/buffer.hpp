#pragma once

#include <GL/glew.h>

#include <span>

namespace GlWrapper
{

template <class T>
class Buffer
{
public:
  Buffer() = delete;

  Buffer(const std::span<T> buffer)
  {
    // check lib types match inbuilt types
    static_assert(sizeof(uint32_t) == sizeof(GLuint), "GLuint is expected to be 32bits in size");

    glGenBuffers(1, &id_);
    bind();
    glBufferData(
      GL_SHADER_STORAGE_BUFFER, buffer.size() * sizeof(T), buffer.data(), GL_DYNAMIC_DRAW);
  }

  Buffer(const T & buffer)
  {
    // check lib types match inbuilt types
    static_assert(sizeof(uint32_t) == sizeof(GLuint), "GLuint is expected to be 32bits in size");

    glGenBuffers(1, &id_);
    bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T), &buffer, GL_DYNAMIC_DRAW);
  }

  ~Buffer() { glDeleteBuffers(1, &id_); }
  void bind() const { glBindBuffer(GL_SHADER_STORAGE_BUFFER, id_); }
  void set_binding_base(int32_t location)
  {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, id_);
  }
  void unbind() const { glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); }

private:
  uint32_t id_;
};

};  // namespace GlWrapper

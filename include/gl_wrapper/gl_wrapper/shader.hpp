#pragma once
#include <GL/glew.h>

#include <string>

namespace GlWrapper
{

class Shader
{
public:
  Shader(const uint32_t type, const std::string & source)
  {
    static_assert(sizeof(uint32_t) == sizeof(GLuint), "GLuint is expected to be 32bits in size");

    id_ = glCreateShader(type);
    source_ = source;
  }
  ~Shader() { glDeleteShader(id_); }

  bool compile() const
  {
    const char * raw_source = source_.data();
    glShaderSource(id_, 1, &raw_source, nullptr);
    glCompileShader(id_);
    return has_compiled();
  }

  std::string get_compilation_log() const
  {
    int length;
    glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &length);

    std::string message;

    if (length != 0) {
      message.resize(length);
      glGetShaderInfoLog(id_, length, &length, message.data());
    }

    return message;
  }

  uint32_t get_id() const { return id_; }

  bool has_compiled() const
  {
    int32_t result;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &result);

    return result == GL_TRUE;
  }

private:
  uint32_t id_;
  std::string source_;
};

};  // namespace GlWrapper

#pragma once

#include <GL/glew.h>

#include <gl_wrapper/shader.hpp>

namespace GlWrapper
{
class ShaderProgram
{
public:
  ShaderProgram() { id_ = glCreateProgram(); }
  ~ShaderProgram() { glDeleteProgram(id_); }

  bool attach_shader(const Shader & shader) const
  {
    if (!shader.has_compiled()) {
      return false;
    }

    glAttachShader(id_, shader.get_id());

    return true;
  }

  bool link() const
  {
    glLinkProgram(id_);

    glValidateProgram(id_);

    return is_valid();
  }

  bool is_valid() const
  {
    int32_t result;

    glGetProgramiv(id_, GL_VALIDATE_STATUS, &result);

    return result == GL_TRUE;
  }

  void bind() const { glUseProgram(id_); }
  void unbind() const { glUseProgram(0); }

  void set_uniform_1i(const std::string & name, const int32_t v0)
  {
    glUniform1i(get_uniform_location(name), v0);
  }

private:
  uint32_t id_;

  int get_uniform_location(const std::string & name)
  {
    return glGetUniformLocation(id_, name.c_str());
  }
};
};  // namespace GlWrapper

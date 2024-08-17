#pragma once

#include <GL/glew.h>

#include <array>
#include <gl_wrapper/index_buffer.hpp>
#include <gl_wrapper/shader.hpp>
#include <gl_wrapper/shader_program.hpp>
#include <gl_wrapper/vertex_array.hpp>
#include <gl_wrapper/vertex_buffer.hpp>
#include <string>
#include <string_view>
#include <utility>

namespace Slimey
{
namespace Rendering
{
// An array of 4 vectors which represents 4 vertices.
// In this situation the vertex specification is just
// position on the screen relative to the centre of a window context
static std::array<GLfloat, 8> VERTICES = {
  -1.0f, -1.0f,  // 0
  1.0f,  -1.0f,  // 1
  1.0f,  1.0f,   // 2
  -1.0f, 1.0f    // 3
};

// an array specifying which vertices will be used in the graphics triangle polygons
static std::array<std::array<GLuint, 3>, 2> INDICIES = {{
  {0, 1, 2},  //0
  {2, 3, 0}   //1
}};

class Renderer
{
public:
  Renderer()
  : vertex_buffer_(GlWrapper::VertexBuffer<GLfloat>(VERTICES)),
    index_buffer_(GlWrapper::IndexBuffer(INDICIES))
  {
    vertex_buffer_.get_layout().append<float>(2);

    vertex_array_.set_buffer(vertex_buffer_);
  }

  std::pair<int32_t, std::string> initialise(
    const std::string & fragment_shader_code, const std::string & vertex_shader_code,
    const int32_t trail_base)
  {
    GlWrapper::Shader fragment_shader(GL_FRAGMENT_SHADER, fragment_shader_code);
    if (!fragment_shader.compile()) {
      return std::make_pair<int32_t, std::string>(-4, fragment_shader.get_compilation_log());
    }

    GlWrapper::Shader vertex_shader(GL_VERTEX_SHADER, vertex_shader_code);
    if (!vertex_shader.compile()) {
      return std::make_pair<int32_t, std::string>(-5, vertex_shader.get_compilation_log());
    }

    render_program_.attach_shader(fragment_shader);
    render_program_.attach_shader(vertex_shader);
    render_program_.link();
    render_program_.bind();
    render_program_.set_uniform_1i("trail_map", trail_base);

    return std::make_pair<int32_t, std::string>(0, "");
  }

  void draw()
  {
    // Draw the vertex block
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_program_.bind();
    vertex_array_.bind();
    index_buffer_.bind();
    glDrawElements(GL_TRIANGLES, index_buffer_.get_vertex_count(), GL_UNSIGNED_INT, nullptr);
    index_buffer_.unbind();
    vertex_array_.unbind();
  }

private:
  GlWrapper::ShaderProgram render_program_;
  GlWrapper::VertexBuffer<GLfloat> vertex_buffer_;
  GlWrapper::VertexArray<GLfloat> vertex_array_;
  GlWrapper::IndexBuffer index_buffer_;
};
};  // namespace Rendering

};  // namespace Slimey

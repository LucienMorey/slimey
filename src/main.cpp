#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <gl_wrapper/index_buffer.hpp>
#include <gl_wrapper/shader.hpp>
#include <gl_wrapper/shader_program.hpp>
#include <gl_wrapper/vertex_array.hpp>
#include <gl_wrapper/vertex_buffer.hpp>
#include <iostream>
#include <sstream>

int main()
{
  /* Initialize the library */
  if (!glfwInit()) {
    std::cerr << "cant init glfw" << std::endl;
    return -1;
  }

  /* Create a windowed mode window and its OpenGL context */
  GLFWwindow * window = nullptr;
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  // initialise glew
  if (glewInit() != GLEW_OK) {
    std::cerr << "cant init glew" << std::endl;
    return -2;
  }

  // An array of 4 vectors which represents 4 vertices.
  // In this situation the vertex specification is just
  // position on the screen relative to the centre of a window context
  static std::array<GLfloat, 8> vertices = {
    -1.0f, -1.0f,  // 0
    1.0f,  -1.0f,  // 1
    1.0f,  1.0f,   // 2
    -1.0f, 1.0f    // 3
  };

  GlWrapper::VertexBuffer<GLfloat> vertex_buffer(vertices);
  vertex_buffer.get_layout().append<float>(2);

  GlWrapper::VertexArray<GLfloat> vertex_array;
  vertex_array.set_buffer(vertex_buffer);

  // an array specifying which vertices will be used in the graphics triangle polygons
  static std::array<std::array<GLuint, 3>, 2> indices = {{
    {0, 1, 2},  //0
    {2, 3, 0}   //1
  }};

  GlWrapper::IndexBuffer index_buffer(indices);

  std::ifstream fragment_file("shaders/fragment_shader.glsl");
  std::stringstream fragment_source;
  fragment_source << fragment_file.rdbuf();
  GlWrapper::Shader fragment_shader(GL_FRAGMENT_SHADER, fragment_source.str());
  auto compilation_success = fragment_shader.compile();
  if (!compilation_success) {
    std::cerr << fragment_shader.get_compilation_log() << std::endl;
    return -4;
  }

  std::ifstream vertex_file("shaders/vertex_shader.glsl");
  std::stringstream vertex_source;
  vertex_source << vertex_file.rdbuf();
  GlWrapper::Shader vertex_shader(GL_VERTEX_SHADER, vertex_source.str());
  compilation_success = vertex_shader.compile();
  if (!compilation_success) {
    std::cerr << vertex_shader.get_compilation_log() << std::endl;
    return -5;
  }
  GlWrapper::ShaderProgram program;
  program.attach_shader(fragment_shader);
  program.attach_shader(vertex_shader);
  program.link();
  program.bind();

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the triangle !
    vertex_array.bind();
    index_buffer.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    index_buffer.unbind();
    vertex_array.unbind();
    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

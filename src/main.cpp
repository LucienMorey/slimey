#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <gl_wrapper/shader.hpp>
#include <gl_wrapper/shader_program.hpp>
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

  // An array of 3 vectors which represents 3 vertices of a triangle
  static std::array<GLfloat, 6> triangle_vertex_positions = {-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 1.0f};

  GlWrapper::VertexBuffer<GLfloat> triangle_buffer(triangle_vertex_positions);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (const void *)0);
  triangle_buffer.unbind();

  std::ifstream fragment_file("shaders/fragment_shader.glsl");
  std::stringstream fragment_source;
  fragment_source << fragment_file.rdbuf();
  GlWrapper::Shader fragment_shader(GL_FRAGMENT_SHADER, fragment_source.str());
  fragment_shader.compile();

  std::ifstream vertex_file("shaders/vertex_shader.glsl");
  std::stringstream vertex_source;
  vertex_source << vertex_file.rdbuf();
  GlWrapper::Shader vertex_shader(GL_VERTEX_SHADER, vertex_source.str());
  vertex_shader.compile();

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
    triangle_buffer.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);  // Starting from vertex 0; 3 vertices total -> 1 triangle
    triangle_buffer.unbind();
    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

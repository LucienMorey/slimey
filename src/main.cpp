#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <gl_wrapper/index_buffer.hpp>
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
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (const void *)0);
  vertex_buffer.unbind();

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
    index_buffer.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    index_buffer.unbind();
    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

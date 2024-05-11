#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <gl_wrapper/vertex_buffer.hpp>
#include <iostream>

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

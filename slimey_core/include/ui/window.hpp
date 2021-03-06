#ifndef WINDOW_H
#define WINDOW_H

// note include order matters here, will get an error if its the other way around
#include <include/glad/glad.h>
#include <GLFW/glfw3.h>


#include <iostream>

class Window
{
public:
  Window(int width, int height, std::string name, int swapInterval, bool isFullscreen);
  ~Window();

  void use();
  void unuse();

  int windowShouldClose() const { return glfwWindowShouldClose(m_win); }
  int getWidth() const { return m_winWidth; }
  int getHeight() const { return m_winHeight; }
  float getDeltaTime() const
  {
    return m_deltaTime;
  }

private:
  void processInput();

  GLFWwindow * m_win;

  int m_winWidth, m_winHeight;

  float m_deltaTime = 0.0f, m_prevFrame = 0.0f, m_current_time = 0.0f;
};

#endif
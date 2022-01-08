#ifndef WINDOW_H
#define WINDOW_H

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
  void getDeltaTime(float& current_time, float& delta_time) const { delta_time = m_deltaTime; current_time = m_current_time; }

private:
  void processInput();

  GLFWwindow * m_win;

  int m_winWidth, m_winHeight;

  float m_deltaTime = 0.0f, m_prevFrame = 0.0f, m_current_time = 0.0f;
};

#endif
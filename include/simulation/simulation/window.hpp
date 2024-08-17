#pragma once
#include <GLFW/glfw3.h>

#include <string>
#include <utility>

namespace Slimey
{
class Window
{
public:
  Window() : window_(nullptr) {}

  ~Window() { glfwTerminate(); }

  std::pair<int32_t, std::string> initialise(const int32_t width, const int32_t height)
  {
    if (!glfwInit()) {
      return std::make_pair<int32_t, std::string>(-1, "cant init glfw");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(width, height, "Slimey", NULL, NULL);
    if (!window_) {
      return std::make_pair<int32_t, std::string>(-2, "Could not create window");
    }

    glfwMakeContextCurrent(window_);

    glfwSetFramebufferSizeCallback(window_, resize_callback_);

    return std::make_pair<int32_t, std::string>(0, "");
  }

  void update() const
  {
    glfwSwapBuffers(window_);

    glfwPollEvents();
  }

  bool should_close() const { return glfwWindowShouldClose(window_); }

private:
  static void resize_callback_(GLFWwindow * window, int width, int height)
  {
    (void)window;
    glViewport(0, 0, width, height);
  }

  GLFWwindow * window_;
};
}  // namespace Slimey

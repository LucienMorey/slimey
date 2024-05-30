#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include <chrono>
#include <fstream>
#include <gl_wrapper/buffer.hpp>
#include <gl_wrapper/index_buffer.hpp>
#include <gl_wrapper/shader.hpp>
#include <gl_wrapper/shader_program.hpp>
#include <gl_wrapper/texture2d32f.hpp>
#include <gl_wrapper/vertex_array.hpp>
#include <gl_wrapper/vertex_buffer.hpp>
#include <iostream>
#include <random>
#include <simulation/agent.hpp>
#include <sstream>

void window_resize_callback(GLFWwindow * window, int width, int height)
{
  (void)window;
  glViewport(0, 0, width, height);
}

std::string read_text_from_file(std::string file_path)
{
  std::ifstream file(file_path);
  std::stringstream text;
  text << file.rdbuf();
  return text.str();
}

constexpr int32_t SCREEN_WIDTH = 640;
constexpr int32_t SCREEN_HEIGHT = 480;
constexpr uint32_t NUM_AGENTS = 500;
constexpr float AGENT_SPEED = 30.0;
constexpr float EVAPORATION_RATE = 0.6;
constexpr float DIFFUSE_WEIGHT = 0.5;
constexpr int DIFFUSE_RADIUS = 1;

int main()
{
  /* Initialize the library */
  if (!glfwInit()) {
    std::cerr << "cant init glfw" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  /* Create a windowed mode window and its OpenGL context */
  GLFWwindow * window = nullptr;
  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(window, window_resize_callback);

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

  GlWrapper::Shader fragment_shader(
    GL_FRAGMENT_SHADER, read_text_from_file("shaders/fragment_shader.glsl"));
  auto compilation_success = fragment_shader.compile();
  if (!compilation_success) {
    std::cerr << fragment_shader.get_compilation_log() << std::endl;
    return -4;
  }

  GlWrapper::Shader vertex_shader(
    GL_VERTEX_SHADER, read_text_from_file("shaders/vertex_shader.glsl"));
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

  GlWrapper::Texture2d32f texture(SCREEN_WIDTH, SCREEN_HEIGHT);
  texture.set_binding_base(0);
  texture.bind();
  program.set_uniform_1i("texture_sample", 0);

  GlWrapper::Shader agent_shader(GL_COMPUTE_SHADER, read_text_from_file("shaders/agent.glsl"));
  compilation_success = agent_shader.compile();
  if (!compilation_success) {
    std::cerr << agent_shader.get_compilation_log() << std::endl;
    return -6;
  }

  GlWrapper::ShaderProgram agent_program;
  agent_program.attach_shader(agent_shader);
  agent_program.link();
  agent_program.bind();

  std::random_device dev;
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  std::array<Slimey::Agent, NUM_AGENTS> agents;
  for (auto & agent : agents) {
    agent.position.x = dist(dev) * SCREEN_WIDTH;
    agent.position.y = dist(dev) * SCREEN_HEIGHT;
    agent.angle = dist(dev) * 2 * M_PI;
  }

  GlWrapper::Buffer<Slimey::Agent> agent_buffer(agents);
  agent_buffer.set_binding_base(1);
  agent_buffer.bind();

  GlWrapper::Shader trail_shader(GL_COMPUTE_SHADER, read_text_from_file("shaders/trail.glsl"));
  compilation_success = trail_shader.compile();
  if (!compilation_success) {
    std::cerr << trail_shader.get_compilation_log() << std::endl;
    return -7;
  }

  GlWrapper::ShaderProgram trail_program;
  trail_program.attach_shader(trail_shader);
  trail_program.link();
  trail_program.bind();

  auto last_time = std::chrono::steady_clock::now().time_since_epoch().count();

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    auto current_time = std::chrono::steady_clock::now().time_since_epoch().count();
    auto delta_time = (current_time - last_time) / 1e9;
    last_time = current_time;

    // Dispatch sim step
    agent_program.bind();
    agent_buffer.bind();
    agent_program.set_uniform_1i("screen_width", SCREEN_WIDTH);
    agent_program.set_uniform_1i("screen_height", SCREEN_HEIGHT);
    agent_program.set_uniform_1f("speed", AGENT_SPEED);
    agent_program.set_uniform_1f("delta_time", delta_time);
    glDispatchCompute(NUM_AGENTS, 1, 1);
    glMemoryBarrier(
      GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_DYNAMIC_STORAGE_BIT |
      GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);

    // dispatch trail update
    trail_program.bind();
    trail_program.set_uniform_1i("screen_width", SCREEN_WIDTH);
    trail_program.set_uniform_1i("screen_height", SCREEN_HEIGHT);
    trail_program.set_uniform_1f("evaporation_rate", EVAPORATION_RATE);
    trail_program.set_uniform_1f("diffuse_weight", DIFFUSE_WEIGHT);
    trail_program.set_uniform_1i("diffuse_radius", DIFFUSE_RADIUS);
    trail_program.set_uniform_1f("delta_time", delta_time);
    glDispatchCompute(SCREEN_WIDTH, SCREEN_HEIGHT, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // Draw the vertex block
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.bind();
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

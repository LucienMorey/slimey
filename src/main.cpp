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
#include <glm/vec4.hpp>
#include <iostream>
#include <random>
#include <simulation/agent.hpp>
#include <sstream>
#include <vector>

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

void print_error_and_exit(const std::string_view & message, int exit_code)
{
  std::cerr << message << std::endl;
  exit(exit_code);
}

// Window Context parameters
constexpr int32_t SCREEN_WIDTH = 640;
constexpr int32_t SCREEN_HEIGHT = 480;

// An array of 4 vectors which represents 4 vertices.
// In this situation the vertex specification is just
// position on the screen relative to the centre of a window context
static std::array<GLfloat, 8> vertices = {
  -1.0f, -1.0f,  // 0
  1.0f,  -1.0f,  // 1
  1.0f,  1.0f,   // 2
  -1.0f, 1.0f    // 3
};

// an array specifying which vertices will be used in the graphics triangle polygons
static std::array<std::array<GLuint, 3>, 2> indices = {{
  {0, 1, 2},  //0
  {2, 3, 0}   //1
}};

// simulation parameters
constexpr uint32_t NUM_AGENTS = 5000;

// agent parameters
constexpr float LINEAR_SPEED = 30.0;
constexpr float ANGULAR_SPEED = M_PI / 3.0;
constexpr glm::vec4 SPECIES_MASK = {1.0, 1.0, 1.0, 1.0};
constexpr float SENSOR_LOOK_AHEAD = 35.0;
constexpr int SENSOR_RADIUS = 1;
constexpr float SENSOR_OFFSET = M_PI / 3.0;

// trail parameters
constexpr float EVAPORATION_RATE = 0.3;
constexpr float DIFFUSE_WEIGHT = 0.08;
constexpr int DIFFUSE_RADIUS = 1;

int main()
{
  /* Initialize the library */
  if (!glfwInit()) {
    print_error_and_exit("cant init glfw", -1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  /* Create a windowed mode window and its OpenGL context */
  GLFWwindow * window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    print_error_and_exit("Could not create window", -2);
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(window, window_resize_callback);

  // initialise glew
  if (glewInit() != GLEW_OK) {
    print_error_and_exit("cant init glew", -3);
  }

  GlWrapper::VertexBuffer<GLfloat> vertex_buffer(vertices);
  vertex_buffer.get_layout().append<float>(2);

  GlWrapper::VertexArray<GLfloat> vertex_array;
  vertex_array.set_buffer(vertex_buffer);

  GlWrapper::IndexBuffer index_buffer(indices);

  GlWrapper::Shader fragment_shader(
    GL_FRAGMENT_SHADER, read_text_from_file("shaders/fragment_shader.glsl"));
  if (!fragment_shader.compile()) {
    print_error_and_exit(fragment_shader.get_compilation_log(), -4);
  }

  GlWrapper::Shader vertex_shader(
    GL_VERTEX_SHADER, read_text_from_file("shaders/vertex_shader.glsl"));
  if (!vertex_shader.compile()) {
    print_error_and_exit(vertex_shader.get_compilation_log(), -5);
  }

  GlWrapper::ShaderProgram program;
  program.attach_shader(fragment_shader);
  program.attach_shader(vertex_shader);
  program.link();
  program.bind();

  GlWrapper::Texture2d32f<SCREEN_WIDTH, SCREEN_HEIGHT, 0> trail_map;
  trail_map.bind();
  program.set_uniform_1i("trail_map", trail_map.get_base_id());

  GlWrapper::Shader agent_shader(GL_COMPUTE_SHADER, read_text_from_file("shaders/agent.glsl"));
  if (!agent_shader.compile()) {
    print_error_and_exit(agent_shader.get_compilation_log(), -6);
  }

  GlWrapper::ShaderProgram agent_program;
  agent_program.attach_shader(agent_shader);
  agent_program.link();
  agent_program.bind();

  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  std::vector<Slimey::Agent> agents(NUM_AGENTS);
  for (auto & agent : agents) {
    agent.position.x = dist(gen) * SCREEN_WIDTH;
    agent.position.y = dist(gen) * SCREEN_HEIGHT;
    agent.angle = dist(gen) * 2 * M_PI;
    agent.species_mask = SPECIES_MASK;
  }

  Slimey::AgentSettings agent_settings;
  agent_settings.linear_speed = LINEAR_SPEED;
  agent_settings.angular_speed = ANGULAR_SPEED;
  agent_settings.sensor_look_ahead = SENSOR_LOOK_AHEAD;
  agent_settings.sensor_radius = SENSOR_RADIUS;
  agent_settings.sensor_offset = SENSOR_OFFSET;

  GlWrapper::Buffer<Slimey::Agent> agent_buffer(agents);
  agent_buffer.set_binding_base(1);
  agent_buffer.bind();

  GlWrapper::Buffer<Slimey::AgentSettings> agent_settings_buffer(agent_settings);
  agent_settings_buffer.set_binding_base(2);
  agent_settings_buffer.bind();

  GlWrapper::Shader trail_shader(GL_COMPUTE_SHADER, read_text_from_file("shaders/trail.glsl"));
  if (!trail_shader.compile()) {
    print_error_and_exit(trail_shader.get_compilation_log(), -7);
  }

  GlWrapper::ShaderProgram trail_program;
  trail_program.attach_shader(trail_shader);
  trail_program.link();
  trail_program.bind();

  auto last_time = std::chrono::steady_clock::now().time_since_epoch().count() / 1e9;

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    auto current_time = std::chrono::steady_clock::now().time_since_epoch().count() / 1e9;
    auto delta_time = (current_time - last_time);
    last_time = current_time;

    // Dispatch sim step
    agent_program.bind();
    agent_buffer.bind();
    agent_program.set_uniform_1i("screen_width", SCREEN_WIDTH);
    agent_program.set_uniform_1i("screen_height", SCREEN_HEIGHT);
    agent_program.set_uniform_1f("delta_time", delta_time);
    agent_program.set_uniform_1f("current_time", current_time);
    glDispatchCompute((NUM_AGENTS + 1024 - 1) / 1024, 1, 1);
    glMemoryBarrier(
      GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_DYNAMIC_STORAGE_BIT |
      GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);

    // dispatch trail update
    trail_program.bind();
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

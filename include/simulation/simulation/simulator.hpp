#pragma once

#include <stdint.h>

#include <gl_wrapper/buffer.hpp>
#include <gl_wrapper/shader_program.hpp>
#include <gl_wrapper/texture2d32f.hpp>
#include <random>
#include <simulation/agent.hpp>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace Slimey
{
template <size_t width, size_t height, uint32_t agent_count>
class Simulator
{
public:
  Simulator()
  : agents_(std::vector<Agent>(agent_count)),
    width_(width),
    height_(height),
    initialised_(false),
    agent_buffer_(nullptr)
  {
  }
  std::pair<int32_t, std::string> initialise(
    const AgentSettings agent_settings, const TrailSettings trail_settings,
    const std::string & agent_shader_code, const std::string & trail_shader_code)
  {
    // generate agents
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (auto & agent : agents_) {
      agent.position.x = dist(gen) * width;
      agent.position.y = dist(gen) * height;
      agent.angle = dist(gen) * 2 * M_PI;
      agent.species_mask = {1.0, 1.0, 1.0, 1.0};
    }

    GlWrapper::Shader agent_shader(GL_COMPUTE_SHADER, agent_shader_code);
    if (!agent_shader.compile()) {
      return std::make_pair<int32_t, std::string>(-6, agent_shader.get_compilation_log());
    }

    agent_program_.attach_shader(agent_shader);
    agent_program_.link();
    agent_program_.bind();
    agent_program_.set_uniform_1i("trail_map", trail_map_.get_base_id());
    agent_program_.set_uniform_1f("agent_settings.linear_speed", agent_settings.linear_speed);
    agent_program_.set_uniform_1f("agent_settings.angular_speed", agent_settings.angular_speed);
    agent_program_.set_uniform_1i("agent_settings.sensor_radius", agent_settings.sensor_radius);
    agent_program_.set_uniform_1f(
      "agent_settings.sensor_look_ahead", agent_settings.sensor_look_ahead);
    agent_program_.set_uniform_1f("agent_settings.sensor_offset", agent_settings.sensor_offset);

    agent_buffer_ = new GlWrapper::Buffer<Slimey::Agent>(agents_);
    agent_buffer_->set_binding_base(1);
    agent_buffer_->bind();

    GlWrapper::Shader trail_shader(GL_COMPUTE_SHADER, trail_shader_code);
    if (!trail_shader.compile()) {
      return std::make_pair<int32_t, std::string>(-7, trail_shader.get_compilation_log());
    }

    trail_program_.attach_shader(trail_shader);
    trail_program_.link();
    trail_program_.bind();
    trail_program_.set_uniform_1i("trail_map", trail_map_.get_base_id());
    trail_program_.set_uniform_1f(
      "trail_settings.evaporation_rate", trail_settings.evaporation_rate);
    trail_program_.set_uniform_1f("trail_settings.diffuse_weight", trail_settings.diffuse_weight);
    trail_program_.set_uniform_1i("trail_settings.diffuse_radius", trail_settings.diffuse_radius);

    initialised_ = true;

    return std::make_pair<int32_t, std::string>(0, "");
  }

  ~Simulator() { delete agent_buffer_; }

  void step(float current_time, float last_step_length)
  {
    if (!initialised_) {
      return;
    }
    // Dispatch sim step
    agent_program_.bind();
    agent_program_.set_uniform_1f("last_step_length", last_step_length);
    agent_program_.set_uniform_1f("current_time", current_time);
    glDispatchCompute((agents_.size() + 1024 - 1) / 1024, 1, 1);
    glMemoryBarrier(
      GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_DYNAMIC_STORAGE_BIT |
      GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);

    // dispatch trail update
    trail_program_.bind();
    trail_program_.set_uniform_1f("last_step_length", last_step_length);
    glDispatchCompute(trail_map_.get_width(), trail_map_.get_height(), 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  }

  int32_t get_trail_map_base() { return trail_map_.get_base_id(); }

private:
  GlWrapper::Texture2d32f<width, height, 0> trail_map_;
  GlWrapper::ShaderProgram agent_program_;
  GlWrapper::ShaderProgram trail_program_;
  std::vector<Slimey::Agent> agents_;
  size_t width_;
  size_t height_;
  bool initialised_;
  GlWrapper::Buffer<Slimey::Agent> * agent_buffer_;
};
}  // namespace Slimey

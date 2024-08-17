#version 460 core

struct Agent
{
  vec4 species_mask;

  vec2 position;
  float angle;
  float padding_;
};

struct AgentSettings
{
  float linear_speed;
  float angular_speed;

  int sensor_radius;
  float sensor_look_ahead;
  float sensor_offset;
};

layout(rgba32f, binding = 0) uniform image2D trail_map;
layout(std430, binding = 1) buffer Agents { Agent agents[]; };
layout(std430, binding = 2) buffer Settings { AgentSettings agent_settings; };

uniform float last_step_length;
uniform float current_time;

uint hash(uint state)
{
  state ^= 2747636419u;
  state *= 2654435769u;
  state ^= state >> 16;
  state *= 2654435769u;
  state ^= state >> 16;
  state *= 2654435769u;
  return state;
}

float scale_hash(uint state) { return state / 4294967295.0; }

float random_generator(uint seed)
{
  uint hashed = hash(seed);
  return scale_hash(hashed);
}

bool position_out_of_environment(vec2 position, ivec2 environment_dimensions)
{
  return (
    ((position.x > environment_dimensions.x) || (position.x < 0)) ||
    ((position.y > environment_dimensions.y) || (position.y < 0)));
}

float sense(
  Agent agent, float sensor_look_ahead, float sensor_offset, int sensor_radius,
  ivec2 environment_dimensions)
{
  vec2 sense_direction = vec2(cos(agent.angle + sensor_offset), sin(agent.angle + sensor_offset));
  vec2 sensor_location = agent.position + sense_direction * sensor_look_ahead;
  float sum = 0.0;

  // sample all pixels in the radius of the sensor location
  for (int offset_x = -sensor_radius; offset_x <= sensor_radius; offset_x++) {
    for (int offset_y = -sensor_radius; offset_y <= sensor_radius; offset_y++) {
      vec2 tentative_sense_pos = sensor_location + vec2(offset_x, offset_y);
      if (position_out_of_environment(tentative_sense_pos, environment_dimensions)) {
        continue;
      }

      sum += dot(agent.species_mask * 2 - 1, imageLoad(trail_map, ivec2(tentative_sense_pos)).rgba);
    }
  }

  return sum;
}

layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;
void main()
{
  ivec2 environment_dimensions = imageSize(trail_map);
  float random = random_generator(uint(current_time * gl_GlobalInvocationID.x));
  // determine agent instance
  Agent agent = agents[gl_GlobalInvocationID.x];

  // detect nearby agents to track
  float weight_forward = sense(
    agent, agent_settings.sensor_look_ahead, 0, agent_settings.sensor_radius,
    environment_dimensions);
  float weight_counter_clockwise = sense(
    agent, agent_settings.sensor_look_ahead, agent_settings.sensor_offset,
    agent_settings.sensor_radius, environment_dimensions);
  float weight_clockwise = sense(
    agent, agent_settings.sensor_look_ahead, -agent_settings.sensor_offset,
    agent_settings.sensor_radius, environment_dimensions);

  // turn to face discovered agents or try something new if none are around
  if ((weight_forward > weight_counter_clockwise) && (weight_forward > weight_clockwise)) {
    agent.angle += 0.0;
  } else if (weight_counter_clockwise > weight_clockwise) {
    agent.angle += random * agent_settings.angular_speed * last_step_length;
  } else {
    agent.angle -= random * agent_settings.angular_speed * last_step_length;
  }

  // determine velocity
  vec2 velocity = agent_settings.linear_speed * vec2(cos(agent.angle), sin(agent.angle));

  // determine new position
  agent.position = agent.position + velocity * last_step_length;

  // clamp
  if (position_out_of_environment(agent.position, environment_dimensions)) {
    agent.position.x = max(0., min(float(environment_dimensions.x), agent.position.x));
    agent.position.y = max(0., min(float(environment_dimensions.y), agent.position.y));
    agent.angle = random * 6.282;
  }

  // update agent
  agents[gl_GlobalInvocationID.x] = agent;

  // update pixel
  ivec2 pixel_coord = ivec2(int(agent.position.x), int(agent.position.y));
  imageStore(trail_map, pixel_coord, agent.species_mask);
}

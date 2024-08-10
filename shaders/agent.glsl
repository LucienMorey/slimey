#version 460 core

struct Agent
{
  vec2 position;
  float angle;
  float padding_;
  vec4 species_mask;
};
layout(local_size_x = 1, local_size_y = 1) in;

layout(rgba32f, binding = 0) uniform writeonly image2D trail_map;
layout(std430, binding = 1) buffer agent { Agent agents[]; };

uniform int screen_width;
uniform int screen_height;
uniform float speed;
uniform float delta_time;
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

void main()
{
  // determine agent instance
  Agent agent = agents[gl_GlobalInvocationID.x];

  // determine velocity
  vec2 velocity = speed * vec2(cos(agent.angle), sin(agent.angle));

  // determine new position
  vec2 new_position = agent.position + velocity * delta_time;
  float new_angle = agent.angle;

  // clamp
  if (
    ((new_position.x > screen_width) || (new_position.x < 0)) ||
    ((new_position.y > screen_height) || (new_position.y < 0))) {
    new_position.x = max(0., min(float(screen_width), new_position.x));
    new_position.y = max(0., min(float(screen_height), new_position.y));
    new_angle = random_generator(uint(current_time * gl_GlobalInvocationID.x)) * 6.282;
  }

  // update agent
  agent.position = new_position;
  agent.angle = new_angle;
  agents[gl_GlobalInvocationID.x] = agent;
  // update pixel
  ivec2 pixel_coord = ivec2(int(agent.position.x), int(agent.position.y));
  imageStore(trail_map, pixel_coord, agent.species_mask);
}

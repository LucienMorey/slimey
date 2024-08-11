#version 460 core

struct Agent
{
  vec2 position;
  float angle;
  float padding_;
  vec4 species_mask;
  float linear_speed;
  float angular_speed;
  vec2 padding2_;
};
layout(local_size_x = 1, local_size_y = 1) in;

layout(rgba32f, binding = 0) uniform writeonly image2D trail_map;
layout(std430, binding = 1) buffer agent { Agent agents[]; };

uniform int screen_width;
uniform int screen_height;
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

bool position_off_screen(vec2 position)
{
  return (
    ((position.x > screen_width) || (position.x < 0)) ||
    ((position.y > screen_height) || (position.y < 0)));
}

void main()
{
  // determine agent instance
  Agent agent = agents[gl_GlobalInvocationID.x];

  // determine velocity
  vec2 velocity = agent.linear_speed * vec2(cos(agent.angle), sin(agent.angle));

  // determine new position
  agent.position = agent.position + velocity * delta_time;

  // clamp
  if (position_off_screen(agent.position)) {
    agent.position.x = max(0., min(float(screen_width), agent.position.x));
    agent.position.y = max(0., min(float(screen_height), agent.position.y));
    agent.angle = random_generator(uint(current_time * gl_GlobalInvocationID.x)) * 6.282;
  }

  // update agent
  agents[gl_GlobalInvocationID.x] = agent;

  // update pixel
  ivec2 pixel_coord = ivec2(int(agent.position.x), int(agent.position.y));
  imageStore(trail_map, pixel_coord, agent.species_mask);
}

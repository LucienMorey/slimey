#version 460 core

struct Agent
{
  vec4 species_mask;

  vec2 position;
  float angle;
  float linear_speed;

  float angular_speed;
  int sensor_radius;
  float sensor_look_ahead;
  float sensor_offset;
};
layout(local_size_x = 1, local_size_y = 1) in;

layout(rgba32f, binding = 0) uniform image2D trail_map;
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

float sense(Agent agent, float angle_offset)
{
  vec2 sense_direction = vec2(cos(agent.angle + angle_offset), sin(agent.angle + angle_offset));
  vec2 sensor_location = agent.position + sense_direction * agent.sensor_look_ahead;
  float sum = 0.0;

  // sample all pixels in the radius of the sensor location
  for (int offset_x = -agent.sensor_radius; offset_x <= agent.sensor_radius; offset_x++) {
    for (int offset_y = -agent.sensor_radius; offset_y <= agent.sensor_radius; offset_y++) {
      vec2 tentative_sense_pos = sensor_location + vec2(offset_x, offset_y);
      if (position_off_screen(tentative_sense_pos)) {
        continue;
      }

      sum += dot(agent.species_mask * 2 - 1, imageLoad(trail_map, ivec2(tentative_sense_pos)).rgba);
    }
  }

  return sum;
}

void main()
{
  float random = random_generator(uint(current_time * gl_GlobalInvocationID.x));
  // determine agent instance
  Agent agent = agents[gl_GlobalInvocationID.x];

  // detect nearby agents to track
  float weight_forward = sense(agent, 0);
  float weight_counter_clockwise = sense(agent, agent.sensor_offset);
  float weight_clockwise = sense(agent, -agent.sensor_offset);

  // turn to face discovered agents or try something new if none are around
  if ((weight_forward > weight_counter_clockwise) && (weight_forward > weight_clockwise)) {
    agent.angle += 0.0;
  } else if (weight_counter_clockwise > weight_clockwise) {
    agent.angle += random * agent.angular_speed * delta_time;
  } else {
    agent.angle -= random * agent.angular_speed * delta_time;
  }

  // determine velocity
  vec2 velocity = agent.linear_speed * vec2(cos(agent.angle), sin(agent.angle));

  // determine new position
  agent.position = agent.position + velocity * delta_time;

  // clamp
  if (position_off_screen(agent.position)) {
    agent.position.x = max(0., min(float(screen_width), agent.position.x));
    agent.position.y = max(0., min(float(screen_height), agent.position.y));
    agent.angle = random * 6.282;
  }

  // update agent
  agents[gl_GlobalInvocationID.x] = agent;

  // update pixel
  ivec2 pixel_coord = ivec2(int(agent.position.x), int(agent.position.y));
  imageStore(trail_map, pixel_coord, agent.species_mask);
}

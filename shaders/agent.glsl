#version 460 core

struct Agent
{
  vec2 position;
  float angle;
  float padding_;
};
layout(local_size_x = 1, local_size_y = 1) in;

layout(rgba32f, binding = 0) uniform writeonly image2D texture_sample;
layout(binding = 1) buffer agent { Agent agents[]; };

void main()
{
  // determine agent instance
  Agent agent = agents[gl_GlobalInvocationID.x];

  // update pixel
  ivec2 pixel_coord = ivec2(int(agent.position.x), int(agent.position.y));
  imageStore(texture_sample, pixel_coord, vec4(1.0, 1.0, 1.0, 1.0));
}

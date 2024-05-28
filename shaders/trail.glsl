#version 460 core

layout(local_size_x = 1, local_size_y = 1) in;

layout(rgba32f, binding = 0) uniform image2D texture_sample;

uniform float evaporation_rate;
uniform float delta_time;

vec4 evaporate(vec4 pixel_colour)
{
  return max(vec4(0.0), pixel_colour - evaporation_rate * delta_time);
}

void main()
{
  // update pixel
  ivec2 pixel_coord = ivec2(gl_GlobalInvocationID.xy);
  vec4 old_pixel_colour = imageLoad(texture_sample, pixel_coord);
  vec4 new_new_pixel_colour = evaporate(old_pixel_colour);

  imageStore(texture_sample, pixel_coord, new_new_pixel_colour);
}

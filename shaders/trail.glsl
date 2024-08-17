#version 460 core

layout(local_size_x = 1, local_size_y = 1) in;

layout(rgba32f, binding = 0) uniform image2D trail_map;

uniform float evaporation_rate;
uniform float diffuse_weight;
uniform int diffuse_radius;
uniform float delta_time;

vec4 evaporate(vec4 pixel_colour, float reduction)
{
  return max(vec4(0.0), pixel_colour - reduction);
}

// blurs by taking the average of surrounding pixels.
// The size of the blur kernel will depend on the radius considered, ie
// 0 will not blur
// 1 will use a 3x3 kernel
// 2 will use a 5x5 kernel
vec4 blur(ivec2 pixel_coord, int radius)
{
  vec4 sum = vec4(0.0);
  float total_weight = 0.0;
  ivec2 image_size = imageSize(trail_map);
  for (int offset_x = -radius; offset_x <= radius; offset_x++) {
    for (int offset_y = -radius; offset_y <= radius; offset_y++) {
      int sample_x = pixel_coord.x + offset_x;
      int sample_y = pixel_coord.y + offset_y;
      if (
        (sample_x >= 0) && (sample_x <= image_size.x) && (sample_y >= 0) &&
        (sample_y <= image_size.y)) {
        sum += imageLoad(trail_map, ivec2(sample_x, sample_y)).rgba;
        total_weight += 1.0;
      }
    }
  }

  return sum / total_weight;
}

void main()
{
  // update pixel
  ivec2 pixel_coord = ivec2(gl_GlobalInvocationID.xy);
  vec4 old_pixel_colour = imageLoad(trail_map, pixel_coord).rgba;
  vec4 blurred = blur(pixel_coord, diffuse_radius);
  vec4 diffused = old_pixel_colour * (1 - diffuse_weight) + blurred * diffuse_weight;
  vec4 new_pixel_colour = evaporate(diffused, evaporation_rate * delta_time);

  imageStore(trail_map, pixel_coord, new_pixel_colour);
}

#version 460 core

layout(local_size_x = 1, local_size_y = 1) in;

layout(rgba32f, binding = 0) uniform writeonly image2D texture_sample;

void main()
{
  vec4 value = vec4(0.0, 0.0, 0.0, 1.0);
  ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

  value.x = float(texelCoord.x) / (gl_NumWorkGroups.x);
  value.y = float(texelCoord.y) / (gl_NumWorkGroups.y);

  imageStore(texture_sample, texelCoord, value);
}

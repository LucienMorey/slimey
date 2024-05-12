#version 460 core

layout(location = 0) in vec2 position;

out vec2 texture_coordinate;

void main()
{
  // rescale coordinate system for texture mapping
  texture_coordinate = 0.5 * position + vec2(0.5);

  gl_Position = vec4(position, 0.0, 1.0);
}

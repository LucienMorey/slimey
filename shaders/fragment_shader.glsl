#version 460 core

out vec4 color;

in vec2 texture_coordinate;

uniform sampler2D trail_map;

void main() { color = texture(trail_map, texture_coordinate); }

#version 460 core

out vec4 color;

in vec2 texture_coordinate;

uniform sampler2D texture_sample;

void main() { color = texture(texture_sample, texture_coordinate); }

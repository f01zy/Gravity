#version 330 core

in vec3 vertex_normale;
in vec2 vertex_texture_coordinates;
uniform sampler2D sphere_texture;

out vec4 color;

void main() { color = texture(sphere_texture, vertex_texture_coordinates); }

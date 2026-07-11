#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normale;
layout(location = 2) in vec2 texture_coordinates;

out vec3 vertex_normale;
out vec2 vertex_texture_coordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(position, 1.0f);
  vertex_normale = normale;
  vertex_texture_coordinates = texture_coordinates;
}

#version 330 core

in vec3 vertex_normale;
in vec2 vertex_texture_coordinates;

uniform bool is_texture;
uniform sampler2D sphere_texture;

out vec4 color;

void main() {
  if (is_texture) {
    color = texture(sphere_texture, vertex_texture_coordinates);
  } else {
    color = vec4(1.0f, 0.984f, 0.0f, 1.0f);
  }
}

#version 330 core

in vec2 vertex_texture_coordinates;
out vec4 color;

uniform sampler2D text_texture;
uniform vec3 text_color;

void main() {
  vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(text_texture, vertex_texture_coordinates).r);
  color = vec4(text_color, 1.0f) * sampled;
}

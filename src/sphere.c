#include <GL/glew.h>
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>

#include "defines.h"
#include "mesh.h"
#include "shader.h"
#include "sphere.h"
#include "texture.h"

void initialize_sphere(Sphere *sphere, vec3 position, vec3 velocity, float weight, float radius, int sectors, int stacks, const char *texture_path) {
  glm_vec3_copy(position, sphere->position);
  glm_vec3_copy(velocity, sphere->velocity);
  sphere->weight = weight;
  sphere->radius = radius;
  sphere->sectors = sectors;
  sphere->stacks = stacks;
  sphere->vertices.len = 0;
  sphere->indices.len = 0;
  sphere->texture = 0;
  if (texture_path) sphere->texture = create_texture(texture_path);

  int k1, k2;
  float x, y, z, xz;
  float s, t;
  float nx, ny, nz, length_inv = 1.0f / radius;
  float sector_angle, stack_angle;
  float sector_step = PI * 2.0f / (float)sectors;
  float stack_step = PI / (float)stacks;
  size_t vertices_size = sizeof(sphere->vertices.buf) / sizeof(sphere->vertices.buf[0]);
  size_t indices_size = sizeof(sphere->indices.buf) / sizeof(sphere->indices.buf[0]);

  for (int i = 0; i <= stacks; i++) {
    stack_angle = PI / 2.0f - stack_step * (float)i;
    xz = radius * cosf(stack_angle);
    y = radius * sinf(stack_angle);

    for (int j = 0; j <= sectors; j++) {
      if (sphere->vertices.len > vertices_size) break;
      sector_angle = sector_step * (float)j;
      x = xz * sinf(sector_angle);
      z = xz * cosf(sector_angle);
      size_t len = sphere->vertices.len;
      sphere->vertices.buf[len].position[0] = x;
      sphere->vertices.buf[len].position[1] = y;
      sphere->vertices.buf[len].position[2] = z;

      nx = x * length_inv;
      ny = y * length_inv;
      nz = z * length_inv;
      sphere->vertices.buf[len].normal[0] = nx;
      sphere->vertices.buf[len].normal[1] = ny;
      sphere->vertices.buf[len].normal[2] = nz;

      s = (float)j / (float)sectors;
      t = (float)i / (float)stacks;
      sphere->vertices.buf[len].texture_coordinates[0] = s;
      sphere->vertices.buf[len].texture_coordinates[1] = t;
      sphere->vertices.len++;
    }
  }

  for (int i = 0; i < stacks; i++) {
    k1 = i * (sectors + 1);
    k2 = k1 + sectors + 1;
    for (int j = 0; j < sectors; j++, k1++, k2++) {
      if (sphere->indices.len > indices_size) break;
      if (i) {
        size_t len = sphere->indices.len;
        sphere->indices.buf[len][0] = k1;
        sphere->indices.buf[len][1] = k2;
        sphere->indices.buf[len][2] = k1 + 1;
        sphere->indices.len++;
      }
      if (i != (stacks - 1)) {
        size_t len = sphere->indices.len;
        sphere->indices.buf[len][0] = k1 + 1;
        sphere->indices.buf[len][1] = k2;
        sphere->indices.buf[len][2] = k2 + 1;
        sphere->indices.len++;
      }
    }
  }
}

void render_sphere(Sphere *sphere, const Mesh *mesh, unsigned shader_program) {
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  size_t len = sizeof(sphere->indices.buf[0]) / sizeof(sphere->indices.buf[0][0]);
  glm_translate(model, sphere->position);
  set_mat4(shader_program, "model", model);
  set_int(shader_program, "sphere_texture", 0);
  glBindTexture(GL_TEXTURE_2D, sphere->texture);
  glBindVertexArray(mesh->VAO);
  glDrawElements(GL_TRIANGLES, (int)sphere->indices.len * (int)len, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

size_t get_sphere_vertices_size(const Sphere *sphere) { return sizeof(sphere->vertices.buf[0]) * sphere->vertices.len; }

size_t get_sphere_indices_size(const Sphere *sphere) { return sizeof(sphere->indices.buf[0]) * sphere->indices.len; }

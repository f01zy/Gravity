#include <cglm/cglm.h>
#include <glad/gl.h>
#include <math.h>
#include <stdio.h>

#include "defines.h"
#include "sphere.h"

// TODO: выделить буферы под вершины и индексы динамически

SphereInitStatus sphere_initialize(Sphere *sphere, vec3 position, vec3 velocity, float weight, float radius, int sectors, int stacks) {
  if (!sphere) return SPHERE_INIT_MISSING_DATA;

  glm_vec3_copy(position, sphere->position);
  glm_vec3_copy(velocity, sphere->velocity);
  sphere->weight = weight;
  sphere->radius = radius;
  sphere->sectors = sectors;
  sphere->stacks = stacks;
  sphere->vertices.len = 0;
  sphere->indices.len = 0;
  sphere->texture = 0;
  sphere->mesh = 0;

  float length_inv = 1.0f / radius;
  float sector_step = PI * 2.0f / (float)sectors;
  float stack_step = PI / (float)stacks;
  size_t vertices_size = sizeof(sphere->vertices.buf) / sizeof(sphere->vertices.buf[0]);
  size_t indices_size = sizeof(sphere->indices.buf) / sizeof(sphere->indices.buf[0]);

  for (int i = 0; i <= stacks; i++) {
    float stack_angle = PI / 2.0f - stack_step * (float)i;
    float xz = radius * cosf(stack_angle);
    float y = radius * sinf(stack_angle);

    for (int j = 0; j <= sectors; j++) {
      if (sphere->vertices.len > vertices_size) break;
      float sector_angle = sector_step * (float)j;
      float x = xz * sinf(sector_angle);
      float z = xz * cosf(sector_angle);
      size_t len = sphere->vertices.len;
      sphere->vertices.buf[len].position[0] = x;
      sphere->vertices.buf[len].position[1] = y;
      sphere->vertices.buf[len].position[2] = z;

      float nx = x * length_inv;
      float ny = y * length_inv;
      float nz = z * length_inv;
      sphere->vertices.buf[len].normal[0] = nx;
      sphere->vertices.buf[len].normal[1] = ny;
      sphere->vertices.buf[len].normal[2] = nz;

      float s = (float)j / (float)sectors;
      float t = (float)i / (float)stacks;
      sphere->vertices.buf[len].texture_coordinates[0] = s;
      sphere->vertices.buf[len].texture_coordinates[1] = t;
      sphere->vertices.len++;
    }
  }

  for (int i = 0; i < stacks; i++) {
    int k1 = i * (sectors + 1);
    int k2 = k1 + sectors + 1;
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

  return SPHERE_INIT_SUCCESS;
}

size_t get_sphere_vertices_size(const Sphere *sphere) { return sizeof(sphere->vertices.buf[0]) * sphere->vertices.len; }

size_t get_sphere_indices_size(const Sphere *sphere) { return sizeof(sphere->indices.buf[0]) * sphere->indices.len; }

#include <cglm/cglm.h>
#include <glad/gl.h>
#include <math.h>
#include <string.h>

#include "core/defines.h"
#include "resources/sphere.h"

SphereInitStatus sphere_initialize(Sphere *sphere, SphereProperties properties) {
  if (!sphere) return SPHERE_INIT_MISSING_DATA;

  float weight = properties.weight;
  float radius = properties.radius;
  int sectors = properties.sectors;
  int stacks = properties.stacks;

  memset(sphere, 0, sizeof(Sphere));
  glm_vec3_copy(properties.position, sphere->position);
  glm_vec3_copy(properties.velocity, sphere->velocity);
  sphere->weight = weight;
  sphere->radius = radius;
  sphere->sectors = sectors;
  sphere->stacks = stacks;

  float length_inv = 1.0f / radius;
  float sector_step = PI * 2.0f / (float)sectors;
  float stack_step = PI / (float)stacks;

  for (int i = 0; i <= stacks; i++) {
    float stack_angle = PI / 2.0f - stack_step * (float)i;
    float xz = radius * cosf(stack_angle);
    float y = radius * sinf(stack_angle);

    for (int j = 0; j <= sectors; j++) {
      if (sphere->vertices.len >= sphere->vertices.size) {
        sphere->vertices.size = (sphere->vertices.size + 1) * 2;
        sphere->vertices.buf = (Vertice *)realloc(sphere->vertices.buf, sizeof(Vertice) * sphere->vertices.size);
      }
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
      if (sphere->indices.len >= sphere->indices.size) {
        sphere->indices.size = (sphere->indices.size + 1) * 2;
        sphere->indices.buf = (ivec3 *)realloc(sphere->indices.buf, sizeof(ivec3) * sphere->indices.size);
      }
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

void sphere_remove(Sphere *sphere) {
  if (!sphere) return;
  free(sphere->vertices.buf);
  free(sphere->indices.buf);
}

size_t get_sphere_vertices_size(const Sphere *sphere) {
  if (!sphere) return 0;
  return sizeof(sphere->vertices.buf[0]) * sphere->vertices.len;
}

size_t get_sphere_indices_size(const Sphere *sphere) {
  if (!sphere) return 0;
  return sizeof(sphere->indices.buf[0]) * sphere->indices.len;
}

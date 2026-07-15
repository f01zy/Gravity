#include <cglm/cglm.h>
#include <math.h>
#include <stddef.h>

#include "core/defines.h"
#include "grid/grid.h"
#include "physics/physics.h"
#include "resources/sphere.h"

void physics_calculate_grid(Grid *grid, Sphere *spheres, size_t len, float world_scale) {
  int w = grid->resolution[0] + 1;
  int h = grid->resolution[1] + 1;
  float step_x = grid->size[0] / w;
  float step_z = grid->size[1] / h;
  float half_w = w / 2.0f;
  float half_h = h / 2.0f;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      size_t curr_vertice = i * w + j;
      vec3 absolute_position;
      vec3 vertex_position = {
        -half_w + step_x * j,
        0.0f,
        -half_h + step_z * i,
      };
      glm_vec3_copy(vertex_position, absolute_position);
      glm_vec3_add(absolute_position, grid->position, absolute_position);
      glm_vec3_scale(absolute_position, 1.0f / world_scale, absolute_position);
      for (int i = 0; i < len; i++) {
        Sphere *sphere = &spheres[i];
        float dx = absolute_position[0] - sphere->position[0];
        float dz = absolute_position[2] - sphere->position[2];
        float distance = dx * dx + dz * dz;
        float y = -(GRAVITATIONAL_CONSTANT * sphere->weight) / sqrt(distance + GRAVITY_SMOOTH * GRAVITY_SMOOTH);
        vertex_position[1] += y * world_scale;
      }
      glm_vec3_copy(vertex_position, grid->vertices.buf[curr_vertice]);
    }
  }
}

void physics_apply_gravity(Sphere *spheres, size_t len, float time_scale, float deltatime) {
  for (int i = 0; i < len; i++) {
    Sphere *sphere = &spheres[i];
    vec3 total_acceleration = GLM_VEC3_ZERO_INIT;
    for (int j = 0; j < len; j++) {
      if (j == i) continue;
      Sphere *curr = &spheres[j];
      float distance = glm_vec3_distance(curr->position, sphere->position);
      float scalar = GRAVITATIONAL_CONSTANT * curr->weight / (distance * distance);
      vec3 direction;
      vec3 curr_acceleration;
      glm_vec3_sub(curr->position, sphere->position, direction);
      glm_normalize(direction);
      glm_vec3_scale(direction, scalar, curr_acceleration);
      glm_vec3_add(total_acceleration, curr_acceleration, total_acceleration);
    }
    vec3 accumulative_velocity;
    glm_vec3_scale(total_acceleration, deltatime * time_scale, accumulative_velocity);
    glm_vec3_add(sphere->velocity, accumulative_velocity, sphere->velocity);
  }
}

void physics_move_spheres(Sphere *spheres, size_t len, float time_scale, float deltatime) {
  for (int i = 0; i < len; i++) {
    Sphere *sphere = &spheres[i];
    vec3 distance_traveled;
    glm_vec3_scale(sphere->velocity, deltatime * time_scale, distance_traveled);
    glm_vec3_add(sphere->position, distance_traveled, sphere->position);
  }
}

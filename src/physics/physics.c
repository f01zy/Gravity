#include <cglm/cglm.h>

#include "core/defines.h"
#include "physics/physics.h"
#include "resources/sphere.h"

void physics_apply_gravity(Sphere *spheres, size_t len, float deltatime) {
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
    glm_vec3_scale(total_acceleration, deltatime * TIME_SPEED, accumulative_velocity);
    glm_vec3_add(sphere->velocity, accumulative_velocity, sphere->velocity);
  }
}

void physics_move_spheres(Sphere *spheres, size_t len, float deltatime) {
  for (int i = 0; i < len; i++) {
    Sphere *sphere = &spheres[i];
    vec3 distance_traveled;
    glm_vec3_scale(sphere->velocity, deltatime * TIME_SPEED, distance_traveled);
    glm_vec3_add(sphere->position, distance_traveled, sphere->position);
  }
}

#include <cglm/cglm.h>
#include <stdio.h>

#include "cglm/vec3.h"
#include "defines.h"
#include "physics.h"
#include "resource_manager.h"
#include "sphere.h"

void physics_apply_gravity(ResourceManager *resource_manager, float deltatime) {
  for (int i = 0; i < resource_manager->spheres.len; i++) {
    Sphere *sphere = &resource_manager->spheres.buf[i];
    vec3 total_acceleration = GLM_VEC3_ZERO_INIT;
    for (int j = 0; j < resource_manager->spheres.len; j++) {
      if (j == i) continue;
      Sphere *curr = &resource_manager->spheres.buf[j];
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
    glm_vec3_scale(total_acceleration, deltatime, accumulative_velocity);
    glm_vec3_add(sphere->velocity, accumulative_velocity, sphere->velocity);
  }
}

void physics_move_spheres(const ResourceManager *resource_manager, float deltatime) {
  for (int i = 0; i < resource_manager->spheres.len; i++) {
    Sphere *sphere = &resource_manager->spheres.buf[i];
    vec3 distance_traveled;
    glm_vec3_scale(sphere->velocity, deltatime, distance_traveled);
    glm_vec3_add(sphere->position, distance_traveled, sphere->position);
    printf("Sphere %d velocity: (%f, %f, %f)\n", i, sphere->velocity[0], sphere->velocity[1], sphere->velocity[2]);
  }
}

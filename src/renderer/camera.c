#include <cglm/cglm.h>
#include <math.h>

#include "renderer/camera.h"

vec3 world_up = {0.0f, 1.0f, 0.0f};
vec3 target = {0.0f, 0.0f, 0.0f};

void camera_initialize(Camera *camera) {
  camera->fov = 45.0f;
  camera->yaw = 0.0f;
  camera->pitch = 0.0f;
  camera->radius = 10.0f;
  camera_update_position(camera);
}

void camera_update_position(Camera *camera) {
  vec3 temp, direction;
  temp[0] = camera->radius * cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
  temp[1] = camera->radius * sin(glm_rad(camera->pitch));
  temp[2] = camera->radius * sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
  glm_vec3_copy(temp, camera->position);
  glm_vec3_sub(target, camera->position, camera->direction);
  glm_normalize(camera->direction);
  glm_cross(camera->direction, world_up, camera->right);
  glm_normalize(camera->right);
  glm_cross(camera->right, camera->direction, camera->up);
  glm_normalize(camera->up);
}

void camera_get_view_matrix(Camera *camera, mat4 view) { return glm_lookat(camera->position, target, camera->up, view); }

#ifndef CAMERA_INCLUDED
#define CAMERA_INCLUDED

#include <cglm/cglm.h>

typedef struct {
  vec3 position;
  vec3 direction;
  vec3 up;
  vec3 right;
  float yaw;
  float pitch;
  float radius;
  float fov;
} Camera;

void camera_initialize(Camera *camera);
void camera_update_position(Camera *camera);
void camera_get_view_matrix(Camera *camera, mat4 view);

#endif

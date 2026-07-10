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

void initialize_camera(Camera *camera);
void update_position(Camera *camera);
void get_view_matrix(Camera *camera, mat4 view);

#endif

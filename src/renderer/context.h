#ifndef CONTEXT_INCLUDED
#define CONTEXT_INCLUDED

#include <cglm/cglm.h>

#include "core/time.h"
#include "grid/grid.h"
#include "renderer/camera.h"
#include "resources/resource_manager.h"

typedef struct {
  ResourceManager *resource_manager;
  Camera *camera;
  Grid *grid;
  Time *time;
  vec2 window_size;
  const char *window_title;
  float time_scale;
  float world_scale;
} Context;

#endif

#ifndef CONTEXT_INCLUDED
#define CONTEXT_INCLUDED

#include <cglm/cglm.h>
#include <stdbool.h>
#include <stdint.h>

#include "core/time.h"
#include "grid/grid.h"
#include "renderer/camera.h"
#include "resources/resource_manager.h"

typedef struct {
  ResourceManager *resource_manager;
  Camera *camera;
  Grid *grid;
  Time *time;
  float time_scale;
  float world_scale;
  bool is_hud;
  uint32_t base_shader_pipeline_id;
  uint32_t text_shader_pipeline_id;
  uint32_t grid_shader_pipeline_id;
  uint32_t font_id;
  uint32_t text_mesh_id;
} Context;

bool context_initialize(Context *ctx, int argc, char **argv);
void context_remove(const Context *ctx);

#endif

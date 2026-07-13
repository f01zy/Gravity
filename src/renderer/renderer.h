#ifndef RENDERER_INCLUDED
#define RENDERER_INCLUDED

#include <cglm/cglm.h>
#include <stdint.h>

#include "renderer/camera.h"
#include "resources/resource_manager.h"

typedef struct {
  const char *text;
  vec2 position;
  float scale;
  vec3 color;
} TextProperties;

void render_text(const ResourceManager *resource_manager, uint32_t font_id, uint32_t font_mesh_id, uint32_t shader_pipeline_id, TextProperties properties);
void render_scene(const ResourceManager *resource_manager, Camera *camera, uint32_t shader_pipeline_id);

#endif

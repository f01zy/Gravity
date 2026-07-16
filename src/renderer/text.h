#ifndef TEXT_INCLUDED
#define TEXT_INCLUDED

#include <cglm/cglm.h>
#include <stdint.h>

#include "context/context.h"

typedef struct {
  const char *text;
  vec2 position;
  float scale;
  vec3 color;
} TextProperties;

typedef struct {
  uint32_t font_id;
  uint32_t mesh_id;
  uint32_t shader_pipeline_id;
} TextResources;

void render_text(const Context *ctx, TextResources resources, TextProperties properties);

#endif

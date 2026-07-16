#ifndef RENDERER_INCLUDED
#define RENDERER_INCLUDED

#include "context/context.h"
#include <stdint.h>

void renderer_render_scene(const Context *ctx, uint32_t shader_pipeline_id, uint32_t grid_shader_pipeline_id);

#endif

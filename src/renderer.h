#ifndef RENDERER_INCLUDED
#define RENDERER_INCLUDED

#include "resource_manager.h"
#include <stdint.h>

void render_sphere(ResourceManager *resource_manager, uint32_t sphere_id, uint32_t shader_pipeline_id);

#endif

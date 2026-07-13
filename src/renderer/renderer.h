#ifndef RENDERER_INCLUDED
#define RENDERER_INCLUDED

#include <stdint.h>

#include "renderer/camera.h"
#include "resources/resource_manager.h"

void renderer_render_scene(const ResourceManager *resource_manager, Camera *camera, uint32_t shader_pipeline_id);

#endif

#ifndef RENDERER_INCLUDED
#define RENDERER_INCLUDED

#include <stdint.h>

#include "camera.h"
#include "resource_manager.h"

void render_sphere(const ResourceManager *resource_manager, uint32_t sphere_id, uint32_t shader_pipeline_id);
void render_scene(const ResourceManager *resource_manager, Camera *camera, uint32_t shader_pipeline_id);

#endif

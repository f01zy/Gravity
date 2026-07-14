#ifndef GRID_INCLUDED
#define GRID_INCLUDED

#include <cglm/cglm.h>
#include <stddef.h>
#include <stdint.h>

#include "resources/resource_manager.h"

typedef struct {
  vec3 position;
  vec2 size;
  uint32_t mesh_id;

  struct {
    vec3 *buf;
    size_t len;
  } vertices;

  struct {
    vec2 *buf;
    size_t len;
  } indices;
} Grid;

void grid_update_vertices(const Grid *grid, const ResourceManager *resource_manager);
void grid_initialize(Grid *grid, ResourceManager *resource_manager, vec3 position, vec2 size);

#endif

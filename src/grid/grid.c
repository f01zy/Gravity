#include <cglm/cglm.h>
#include <stdint.h>
#include <string.h>

#include "core/defines.h"
#include "core/graphics.h"
#include "grid/grid.h"
#include "resources/mesh.h"
#include "resources/resource_manager.h"

void grid_update_vertices(const Grid *grid, const ResourceManager *resource_manager) {
  const Mesh *mesh = res_get_mesh(resource_manager, grid->mesh_id);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(*grid->vertices.buf) * grid->vertices.len, grid->vertices.buf);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void grid_initialize(Grid *grid, ResourceManager *resource_manager, vec3 position, vec2 size, ivec2 resolution) {
  glm_vec3_copy(position, grid->position);
  glm_ivec2_copy(resolution, grid->resolution);
  glm_vec2_copy(size, grid->size);

  Attribute attributes[] = {(Attribute){.size = 3, .type = GL_FLOAT}};
  int w = resolution[0];
  int h = resolution[1];
  size_t vertices_count = (w + 1) * (h + 1);
  size_t indices_count = w * h * 2 + w + h;
  size_t vertices_size = sizeof(*grid->vertices.buf) * vertices_count;
  size_t indices_size = sizeof(*grid->indices.buf) * indices_count;

  grid->vertices.len = vertices_count;
  grid->indices.len = indices_count;
  grid->vertices.buf = malloc(vertices_size);
  grid->indices.buf = malloc(indices_size);

  size_t curr_indice = 0;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      int curr_vertex = i * (w + 1) + j;
      if (j < w - 1) {
        grid->indices.buf[curr_indice][0] = curr_vertex;
        grid->indices.buf[curr_indice][1] = curr_vertex + 1;
        curr_indice++;
      }
      if (i < h - 1) {
        grid->indices.buf[curr_indice][0] = curr_vertex;
        grid->indices.buf[curr_indice][1] = curr_vertex + w + 1;
        curr_indice++;
      }
    }
  }

  uint32_t mesh_id = res_create_mesh(resource_manager, (MeshProperties){
                                                         .vertices = NULL,
                                                         .vertices_size = vertices_size,
                                                         .indices = (uint32_t *)grid->indices.buf,
                                                         .indices_size = indices_size,
                                                         .attributes = attributes,
                                                         .attributes_size = 1,
                                                         .render_mode = GL_DYNAMIC_DRAW,
                                                       });

  if (mesh_id == INVALID_RESOURCE) {
    grid_remove(grid);
    return;
  }

  grid->mesh_id = mesh_id;
}

void grid_remove(const Grid *grid) {
  free(grid->vertices.buf);
  free(grid->indices.buf);
}

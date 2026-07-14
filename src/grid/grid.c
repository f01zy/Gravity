#include <cglm/cglm.h>
#include <stdint.h>
#include <string.h>

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

void grid_initialize(Grid *grid, ResourceManager *resource_manager, vec3 position, vec2 size) {
  glm_vec3_copy(position, grid->position);
  glm_vec2_copy(size, grid->size);

  Attribute attributes[] = {(Attribute){.size = 3, .type = GL_FLOAT}};
  size_t curr_indice = 0;
  size_t vertices_count = (size[0] + 1) * (size[1] + 1);
  size_t vertices_size = sizeof(*grid->vertices.buf) * vertices_count;
  size_t indices_count = size[0] * size[1] * 2;
  size_t indices_size = sizeof(*grid->indices.buf) * indices_count;

  grid->vertices.len = vertices_count;
  grid->vertices.buf = malloc(vertices_size);
  grid->indices.len = indices_count;
  grid->indices.buf = malloc(indices_size);
  memset(grid->vertices.buf, 0, vertices_size);

  for (int i = 0; i < size[1]; i++) {
    for (int j = 0; j < size[0]; j++) {
      if (j < size[0] - 1) {
        grid->indices.buf[curr_indice][0] = j;
        grid->indices.buf[curr_indice][1] = j + 1;
        curr_indice++;
      }
      if (i < size[1] - 1) {
        grid->indices.buf[curr_indice][0] = j;
        grid->indices.buf[curr_indice][1] = j + size[0] + 1;
        curr_indice++;
      }
    }
  }

  uint32_t mesh_id = res_create_mesh(resource_manager, (MeshProperties){
                                                         .vertices = (float *)grid->vertices.buf,
                                                         .vertices_size = vertices_size,
                                                         .indices = (uint32_t *)grid->indices.buf,
                                                         .indices_size = indices_size,
                                                         .attributes = attributes,
                                                         .attributes_size = 1,
                                                         .render_mode = GL_STREAM_DRAW,
                                                       });
}

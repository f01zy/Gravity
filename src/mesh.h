#ifndef MESH_INCLUDED
#define MESH_INCLUDED

#include <cglm/cglm.h>
#include <stddef.h>

typedef struct {
  vec3 position;
  vec3 normal;
  vec2 texture_coordinates;
} __attribute__((packed)) Vertice;

typedef struct {
  unsigned VAO;
  unsigned VBO;
  unsigned IBO;
} Mesh;

void initialize_mesh(Mesh *mesh, Vertice *vertices, size_t vertices_size, ivec3 *indices, size_t indices_size);

#endif

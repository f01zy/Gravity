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

typedef enum {
  MESH_INIT_MISSING_DATA,
  MESH_INIT_SUCCESS,
} MeshInitStatus;

MeshInitStatus mesh_initialize(Mesh *mesh, Vertice *vertices, size_t vertices_size, ivec3 *indices, size_t indices_size);
void mesh_remove(Mesh *mesh);

#endif

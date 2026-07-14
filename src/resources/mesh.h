#ifndef MESH_INCLUDED
#define MESH_INCLUDED

#include <cglm/cglm.h>
#include <stddef.h>

#include "core/graphics.h"

typedef struct {
  vec3 position;
  vec3 normal;
  vec2 texture_coordinates;
} __attribute__((packed)) Vertice;

typedef struct {
  size_t size;
  GLenum type;
} Attribute;

typedef struct {
  unsigned VAO;
  unsigned VBO;
  unsigned IBO;
} Mesh;

typedef struct {
  Vertice *vertices;
  size_t vertices_size;
  ivec3 *indices;
  size_t indices_size;
  Attribute *attributes;
  size_t attributes_size;
  GLenum render_mode;
} MeshProperties;

typedef enum {
  MESH_INIT_MISSING_DATA,
  MESH_INIT_RENDER_MODE_ERROR,
  MESH_INIT_ATTRIBUTE_TYPE_ERROR,
  MESH_INIT_SUCCESS,
} MeshInitStatus;

MeshInitStatus mesh_initialize(Mesh *mesh, MeshProperties properties);
void mesh_remove(const Mesh *mesh);

#endif

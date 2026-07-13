#include <glad/gl.h>
#include <stddef.h>

#include "core/utility.h"
#include "resources/mesh.h"

MeshInitStatus mesh_initialize(Mesh *mesh, MeshProperties properties) {
  Vertice *vertices = properties.vertices;
  size_t vertices_size = properties.vertices_size;
  ivec3 *indices = properties.indices;
  size_t indices_size = properties.indices_size;
  Attribute *attributes = properties.attributes;
  size_t attributes_size = properties.attributes_size;
  GLenum render_mode = properties.render_mode;

  if (!mesh || !attributes) return MESH_INIT_MISSING_DATA;
  if (render_mode != GL_STATIC_DRAW && render_mode != GL_DYNAMIC_DRAW && render_mode != GL_STREAM_DRAW) return MESH_INIT_RENDER_MODE_ERROR;
  for (int i = 0; i < attributes_size; i++) {
    size_t type_size = utility_get_opengl_type_size(attributes[i].type);
    if (type_size == 0) return MESH_INIT_ATTRIBUTE_TYPE_ERROR;
  }

  glGenVertexArrays(1, &mesh->VAO);
  glBindVertexArray(mesh->VAO);

  glGenBuffers(1, &mesh->VBO);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, render_mode);
  size_t stride = 0;
  size_t offset = 0;
  for (int i = 0; i < attributes_size; i++) {
    stride += utility_get_opengl_type_size(attributes[i].type) * attributes[i].size;
  }
  for (int i = 0; i < attributes_size; i++) {
    const Attribute *attribute = &attributes[i];
    glVertexAttribPointer(i, attribute->size, attribute->type, false, stride, (void *)offset);
    glEnableVertexAttribArray(i);
    offset += utility_get_opengl_type_size(attribute->type) * attribute->size;
  }

  if (indices) {
    glGenBuffers(1, &mesh->IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);
  }

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return MESH_INIT_SUCCESS;
}

void mesh_remove(const Mesh *mesh) {
  glDeleteVertexArrays(1, &mesh->VAO);
  glDeleteBuffers(1, &mesh->VBO);
  glDeleteBuffers(1, &mesh->IBO);
}

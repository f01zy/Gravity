#include <GL/glew.h>

#include "mesh.h"

void initialize_mesh(Mesh *mesh, Vertice *vertices, size_t vertices_size, ivec3 *indices, size_t indices_size) {
  glGenVertexArrays(1, &mesh->VAO);
  glBindVertexArray(mesh->VAO);

  glGenBuffers(1, &mesh->VBO);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 8, (void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float) * 8, (void *)(sizeof(float) * 3));
  glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(float) * 8, (void *)(sizeof(float) * 6));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glGenBuffers(1, &mesh->IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

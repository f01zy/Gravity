#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "core/graphics.h"
#include "utility.h"

bool utility_read_file(const char *path, char *buf, size_t size) {
  FILE *file = fopen(path, "r");
  if (!file) return false;
  size_t count = fread(buf, sizeof(char), size - 1, file);
  buf[count] = '\0';
  fclose(file);
  return true;
}

size_t utility_get_opengl_type_size(GLenum type) {
  switch (type) {
  case GL_FLOAT:
    return sizeof(GLfloat);
  case GL_INT:
    return sizeof(GLint);
  case GL_UNSIGNED_INT:
    return sizeof(GLuint);
  case GL_SHORT:
    return sizeof(GLshort);
  case GL_UNSIGNED_SHORT:
    return sizeof(GLushort);
  case GL_BYTE:
    return sizeof(GLbyte);
  case GL_UNSIGNED_BYTE:
    return sizeof(GLubyte);
  case GL_DOUBLE:
    return sizeof(GLdouble);
  default:
    return 0;
  }
}

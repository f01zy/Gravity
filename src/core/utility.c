#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "core/graphics.h"
#include "utility.h"

bool utility_read_file(const char *path, char *buf, size_t size) {
  FILE *file = fopen(path, "r");
  if (!file) return false;
  buf[fread(buf, sizeof(char), size - 1, file)] = '\0';
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

void utility_help_message() {
  printf("Usage: ./gravity <x> <y> <z> <vx> <vy> <vz> <mass> <radius>\n");
  printf("Arguments:\n");
  printf("  x, y, z      Initial position coordinates (float)\n");
  printf("  vx, vy, vz   Initial velocity vector (float)\n");
  printf("  mass         Object mass (double, > 0)\n");
  printf("  radius       Physical radius of the body (double, > 0)\n");
}

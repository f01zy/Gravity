#ifndef UTILITY_INCLUDED
#define UTILITY_INCLUDED

#include <glad/gl.h>
#include <stdbool.h>
#include <stddef.h>

bool utility_read_file(const char *path, char *buf, size_t size);
size_t utility_get_opengl_type_size(GLenum type);

#endif

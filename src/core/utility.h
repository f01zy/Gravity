#ifndef UTILITY_INCLUDED
#define UTILITY_INCLUDED

#include <stdbool.h>
#include <stddef.h>

#include "core/graphics.h"

bool utility_read_file(const char *path, char *buf, size_t size);
size_t utility_get_opengl_type_size(GLenum type);
void utility_help_message();

#endif

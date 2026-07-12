#ifndef TEXTURE_INCLUDED
#define TEXTURE_INCLUDED

#include <stdint.h>

typedef enum {
  TEXTURE_INIT_MISSING_DATA,
  TEXTURE_INIT_FAILED_LOAD,
  TEXTURE_INIT_SUCCESS,
} TextureInitStatus;

TextureInitStatus texture_initialize(uint32_t *texture, const char *path);
void texture_remove(uint32_t texture);

#endif

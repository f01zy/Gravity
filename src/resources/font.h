#ifndef FONT_INCLUDED
#define FONT_INCLUDED

#include <cglm/cglm.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint32_t texture;
  uint32_t advance;
  ivec2 size;
  ivec2 bearing;
} Character;

typedef struct {
  Character characters[255];
} Font;

typedef enum {
  FONT_INIT_MISSING_DATA,
  FONT_INIT_FAILED_INIT_FT,
  FONT_INIT_FAILED_LOAD_FONT,
  FONT_INIT_SUCCESS,
} FontInitStatus;

FontInitStatus font_initialize(Font *font, const char *path, int size);
void font_remove(Font *font);

#endif

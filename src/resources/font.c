#include <freetype/freetype.h>
#include <glad/gl.h>
#include <stdint.h>

#include "resources/font.h"

FontInitStatus font_initialize(Font *font, const char *path, int size) {
  if (!font || !path) return FONT_INIT_MISSING_DATA;
  FT_Library ft;
  FT_Face face;
  if (FT_Init_FreeType(&ft)) return FONT_INIT_FAILED_INIT_FT;
  if (FT_New_Face(ft, path, 0, &face)) return FONT_INIT_FAILED_LOAD_FONT;
  FT_Set_Pixel_Sizes(face, 0, size);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  for (int i = 0; i < 255; i++) {
    if (FT_Load_Char(face, i, FT_LOAD_RENDER)) continue;
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    font->characters.buf[i].advance = face->glyph->advance.x;
    font->characters.buf[i].size[0] = face->glyph->bitmap.width;
    font->characters.buf[i].size[1] = face->glyph->bitmap.rows;
    font->characters.buf[i].bearing[0] = face->glyph->bitmap_left;
    font->characters.buf[i].bearing[1] = face->glyph->bitmap_top;
  }
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
  return FONT_INIT_SUCCESS;
}

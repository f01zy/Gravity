#define STB_IMAGE_IMPLEMENTATION

#include <glad/gl.h>
#include <stb_image.h>
#include <stdint.h>

#include "texture.h"

TextureInitStatus texture_initialize(uint32_t *texture, const char *path) {
  if (!texture || !path) return TEXTURE_INIT_MISSING_DATA;

  int width, height, channels;
  uint8_t *data = stbi_load(path, &width, &height, &channels, 0);
  uint32_t format;

  if (!data) {
    stbi_image_free(data);
    return TEXTURE_INIT_FAILED_LOAD;
  }

  switch (channels) {
  case 1:
    format = GL_RED;
    break;
  case 3:
    format = GL_RGB;
    break;
  case 4:
    format = GL_RGBA;
    break;
  }

  glGenTextures(1, texture);
  glBindTexture(GL_TEXTURE_2D, *texture);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(data);

  return TEXTURE_INIT_SUCCESS;
}

void texture_remove(uint32_t texture) { glDeleteTextures(1, &texture); }

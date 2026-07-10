#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "utility.h"

void read_file_source(const char *path, char *buf, size_t size) {
  FILE *file = fopen(path, "r");
  if (!file) {
    printf("Failed to open file: %s\n", path);
    exit(1);
  }
  size_t count = fread(buf, sizeof(char), size - 1, file);
  buf[count] = '\0';
}

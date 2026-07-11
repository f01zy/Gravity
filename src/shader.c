#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "shader.h"
#include "utility.h"

int success;
char info[512];

void inialize_shader(unsigned shader_program, unsigned shader, const char *path) {
  char buf[MAX_FILE_LEN];
  const char *source = buf;
  read_file_source(path, buf, sizeof(buf));
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, sizeof(info), NULL, info);
    printf("[ERROR] Failed to compile shader: %s\n", info);
    return;
  }
  printf("[LOG] Shader successfully compiled: %s\n", path);
  glAttachShader(shader_program, shader);
}

unsigned create_shader_program(const char *vertex_shader_source, const char *fragment_shader_source) {
  unsigned shader_program = glCreateProgram();
  unsigned vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  unsigned fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  inialize_shader(shader_program, vertex_shader, vertex_shader_source);
  inialize_shader(shader_program, fragment_shader, fragment_shader_source);
  glLinkProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, sizeof(info), NULL, info);
    printf("[ERROR] Failed to link shader program: %s\n", info);
    exit(1);
  }
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  return shader_program;
}

void set_mat4(unsigned shader_program, const char *name, mat4 mat) {
  glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, (float *)mat);
}

void set_int(unsigned shader_program, const char *name, int x) { glUniform1i(glGetUniformLocation(shader_program, name), x); }

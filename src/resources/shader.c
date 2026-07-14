#include <stdbool.h>
#include <stdint.h>

#include "core/defines.h"
#include "core/graphics.h"
#include "core/utility.h"
#include "resources/shader.h"

bool shader_initialize(uint32_t shader_program, uint32_t shader, const char *path) {
  char buf[MAX_FILE_LEN];
  const char *source = buf;
  int success;
  bool status = utility_read_file(path, buf, sizeof(buf));
  if (!status) return false;
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) return false;
  glAttachShader(shader_program, shader);
  return true;
}

PipelineInitStatus shader_pipeline_initialize(ShaderPipeline *shader_pipeline, const char *vertex_shader_path, const char *fragment_shader_path) {
  if (!vertex_shader_path || !fragment_shader_path) return PIPELINE_INIT_MISSING_DATA;
  shader_pipeline->shader_program = glCreateProgram();
  shader_pipeline->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  shader_pipeline->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  uint32_t shader_program = shader_pipeline->shader_program;
  uint32_t vertex_shader = shader_pipeline->vertex_shader;
  uint32_t fragment_shader = shader_pipeline->fragment_shader;
  int success;
  if (!shader_initialize(shader_program, vertex_shader, vertex_shader_path) || !shader_initialize(shader_program, fragment_shader, fragment_shader_path)) {
    return PIPELINE_INIT_SHADER_ERROR;
  }
  glLinkProgram(shader_program);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) return PIPELINE_INIT_FAILED_LINK;
  return PIPELINE_INIT_SUCCESS;
}

void shader_pipeline_remove(const ShaderPipeline *shader_pipeline) { glDeleteProgram(shader_pipeline->shader_program); }

void uniform_set_mat4(uint32_t shader_program, const char *name, mat4 mat) {
  glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, (float *)mat);
}

void uniform_set_vec3(uint32_t shader_program, const char *name, vec3 vec) { glUniform3fv(glGetUniformLocation(shader_program, name), 1, vec); }

void uniform_set_int(uint32_t shader_program, const char *name, int x) { glUniform1i(glGetUniformLocation(shader_program, name), x); }

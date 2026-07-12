#ifndef SHADER_INCLUDED
#define SHADER_INCLUDED

#include <cglm/cglm.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  unsigned vertex_shader;
  unsigned fragment_shader;
  unsigned shader_program;
} ShaderPipeline;

typedef enum {
  PIPELINE_INIT_MISSING_DATA,
  PIPELINE_INIT_SHADER_ERROR,
  PIPELINE_INIT_FAILED_LINK,
  PIPELINE_INIT_SUCCESS,
} PipelineInitStatus;

PipelineInitStatus shader_pipeline_initialize(ShaderPipeline *shader_pipeline, const char *vertex_shader_path, const char *fragment_shader_path);
void shader_pipeline_remove(const ShaderPipeline *shader_pipeline);
void uniform_set_mat4(uint32_t shader_program, const char *name, mat4 mat);
void uniform_set_int(uint32_t shader_program, const char *name, int x);

#endif

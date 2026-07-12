#include <glad/gl.h>
#include <stdint.h>
#include <stdio.h>

#include "renderer.h"
#include "resource_manager.h"
#include "shader.h"

// TODO: нихуя не рендерится

void render_sphere(ResourceManager *resource_manager, uint32_t sphere_id, uint32_t shader_pipeline_id) {
  Sphere *sphere = res_get_sphere(resource_manager, sphere_id);
  const ShaderPipeline *pipeline = res_get_shader_pipeline(resource_manager, shader_pipeline_id);
  const Mesh *mesh = res_get_mesh(resource_manager, sphere->mesh);
  const uint32_t texture = res_get_texture(resource_manager, sphere->texture);

  size_t len = sizeof(sphere->indices.buf[0]) / sizeof(sphere->indices.buf[0][0]);
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  glm_translate(model, sphere->position);
  uniform_set_mat4(pipeline->shader_program, "model", model);
  uniform_set_int(pipeline->shader_program, "sphere_texture", 0);

  glBindTexture(GL_TEXTURE_2D, texture);
  glBindVertexArray(mesh->VAO);
  glDrawElements(GL_TRIANGLES, (int)sphere->indices.len * (int)len, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

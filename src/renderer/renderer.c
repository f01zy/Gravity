#include <glad/gl.h>
#include <stdint.h>
#include <stdio.h>

#include "core/defines.h"
#include "renderer/renderer.h"
#include "resources/resource_manager.h"
#include "resources/shader.h"

void render_sphere(const ResourceManager *resource_manager, uint32_t sphere_id, uint32_t shader_pipeline_id) {
  Sphere *sphere = res_get_sphere(resource_manager, sphere_id);
  const ShaderPipeline *shader_pipeline = res_get_shader_pipeline(resource_manager, shader_pipeline_id);
  const Mesh *mesh = res_get_mesh(resource_manager, sphere->mesh_id);
  const uint32_t texture = res_get_texture(resource_manager, sphere->texture_id);
  size_t len = sizeof(sphere->indices.buf[0]) / sizeof(sphere->indices.buf[0][0]);
  mat4 model = GLM_MAT4_IDENTITY_INIT;

  glUseProgram(shader_pipeline->shader_program);
  glm_translate(model, sphere->position);
  uniform_set_mat4(shader_pipeline->shader_program, "model", model);
  uniform_set_int(shader_pipeline->shader_program, "sphere_texture", 0);

  glBindTexture(GL_TEXTURE_2D, texture);
  glBindVertexArray(mesh->VAO);
  glDrawElements(GL_TRIANGLES, (int)sphere->indices.len * (int)len, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void render_text(const ResourceManager *resource_manager, uint32_t font_id, uint32_t font_mesh_id, uint32_t shader_pipeline_id, TextProperties properties) {}

void render_scene(const ResourceManager *resource_manager, Camera *camera, uint32_t shader_pipeline_id) {
  const ShaderPipeline *shader_pipeline = res_get_shader_pipeline(resource_manager, shader_pipeline_id);

  glUseProgram(shader_pipeline->shader_program);
  mat4 view;
  mat4 projection;
  camera_update_position(camera);
  camera_get_view_matrix(camera, view);
  glm_perspective(camera->fov, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f, projection);
  uniform_set_mat4(shader_pipeline->shader_program, "view", view);
  uniform_set_mat4(shader_pipeline->shader_program, "projection", projection);

  for (int i = 0; i < resource_manager->spheres.len; i++) {
    render_sphere(resource_manager, i, shader_pipeline_id);
  }
}

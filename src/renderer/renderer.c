#include <stdint.h>

#include "core/graphics.h"
#include "grid/grid.h"
#include "renderer/context.h"
#include "renderer/renderer.h"
#include "resources/resource_manager.h"
#include "resources/shader.h"

void renderer_render_grid(Grid *grid, const ResourceManager *resource_manager, uint32_t shader_pipeline_id, mat4 view, mat4 projection) {
  const ShaderPipeline *shader_pipeline = res_get_shader_pipeline(resource_manager, shader_pipeline_id);
  const Mesh *mesh = res_get_mesh(resource_manager, grid->mesh_id);

  if (!shader_pipeline || !mesh) {
    printf("[ERROR] Failed to render the grid, because the resources weren't found\n");
    return;
  }

  glUseProgram(shader_pipeline->shader_program);
  size_t indices_count = sizeof(*grid->indices.buf) / sizeof(*grid->indices.buf[0]) * grid->indices.len;
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  glm_translate(model, grid->position);
  uniform_set_mat4(shader_pipeline->shader_program, "model", model);
  uniform_set_mat4(shader_pipeline->shader_program, "view", view);
  uniform_set_mat4(shader_pipeline->shader_program, "projection", projection);

  glBindVertexArray(mesh->VAO);
  glDrawElements(GL_LINES, indices_count, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);
}

void renderer_render_sphere(const ResourceManager *resource_manager, uint32_t sphere_id, uint32_t shader_pipeline_id, float world_scale) {
  Sphere *sphere = res_get_sphere(resource_manager, sphere_id);
  const ShaderPipeline *shader_pipeline = res_get_shader_pipeline(resource_manager, shader_pipeline_id);
  const Mesh *mesh = res_get_mesh(resource_manager, sphere->mesh_id);
  const uint32_t texture = res_get_texture(resource_manager, sphere->texture_id);

  if (!sphere || !shader_pipeline || !mesh) {
    printf("[ERROR] Failed to render the sphere, because the resources weren't found\n");
    return;
  }

  size_t indices_count = sizeof(*sphere->indices.buf) / sizeof(*sphere->indices.buf[0]) * sphere->indices.len;
  float scaled_radius = sphere->radius * world_scale;
  vec3 scaled_position;
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  glm_vec3_scale(sphere->position, world_scale, scaled_position);
  glm_translate(model, scaled_position);
  glm_scale_uni(model, scaled_radius);
  uniform_set_mat4(shader_pipeline->shader_program, "model", model);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glBindVertexArray(mesh->VAO);
  glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void renderer_render_scene(const Context *ctx, uint32_t base_shader_pipeline_id, uint32_t grid_shader_pipeline_id) {
  mat4 view;
  mat4 projection;
  camera_update_position(ctx->camera);
  camera_get_view_matrix(ctx->camera, view);
  glm_perspective(ctx->camera->fov, ctx->window_size[0] / ctx->window_size[1], 0.1f, 100.0f, projection);

  const ShaderPipeline *base_shader_pipeline = res_get_shader_pipeline(ctx->resource_manager, base_shader_pipeline_id);
  glUseProgram(base_shader_pipeline->shader_program);
  uniform_set_mat4(base_shader_pipeline->shader_program, "view", view);
  uniform_set_mat4(base_shader_pipeline->shader_program, "projection", projection);
  for (int i = 0; i < ctx->resource_manager->spheres.len; i++) {
    renderer_render_sphere(ctx->resource_manager, i, base_shader_pipeline_id, ctx->world_scale);
  }

  renderer_render_grid(ctx->grid, ctx->resource_manager, grid_shader_pipeline_id, view, projection);
}

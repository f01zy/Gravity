#include "game/game.h"
#include "context/context.h"
#include "core/graphics.h"
#include "game/input.h"
#include "physics/physics.h"
#include "renderer/hud.h"
#include "renderer/renderer.h"
#include "renderer/text.h"

void game_loop(GLFWwindow *window, Context *ctx) {
  ResourceManager *resource_manager = ctx->resource_manager;

  while (!glfwWindowShouldClose(window)) {
    if (!time_check_fps(ctx->time)) continue;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    input_handle_keyboard(window);
    physics_apply_gravity(resource_manager->spheres.buf, resource_manager->spheres.len, ctx->time_scale, ctx->time->deltatime);
    physics_move_spheres(resource_manager->spheres.buf, resource_manager->spheres.len, ctx->time_scale, ctx->time->deltatime);
    physics_calculate_grid(ctx->grid, resource_manager->spheres.buf, resource_manager->spheres.len, ctx->world_scale);
    grid_update_vertices(ctx->grid, ctx->resource_manager);
    renderer_render_scene(ctx, ctx->base_shader_pipeline_id, ctx->grid_shader_pipeline_id);
    if (ctx->is_hud) {
      hud_render(ctx, (TextResources){
                        .font_id = ctx->font_id,
                        .mesh_id = ctx->text_mesh_id,
                        .shader_pipeline_id = ctx->text_shader_pipeline_id,
                      });
    }

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  grid_remove(ctx->grid);
  res_remove_all(ctx->resource_manager);
  context_remove(ctx);
  glfwTerminate();
}

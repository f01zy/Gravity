#include <cglm/cglm.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "core/defines.h"
#include "core/graphics.h"
#include "core/time.h"
#include "game/window.h"
#include "grid/grid.h"
#include "physics/physics.h"
#include "renderer/camera.h"
#include "renderer/context.h"
#include "renderer/hud.h"
#include "renderer/renderer.h"
#include "resources/resource_manager.h"

int main() {
  ResourceManager resource_manager = {0};
  Time time = {.fps = FPS};
  Camera camera;
  Grid grid;
  Context ctx = {
    .resource_manager = &resource_manager,
    .camera = &camera,
    .grid = &grid,
    .time = &time,
    .window_size = {WIDTH, HEIGHT},
    .window_title = TITLE,
    .time_scale = TIME_SCALE,
    .world_scale = WORLD_SCALE,
  };
  GLFWwindow *window = window_create(&ctx);
  if (!window) return 1;

  uint32_t base_shader_pipeline_id = res_create_shader_pipeline(&resource_manager, "../src/shaders/base.vert", "../src/shaders/base.frag");
  uint32_t text_shader_pipeline_id = res_create_shader_pipeline(&resource_manager, "../src/shaders/text.vert", "../src/shaders/text.frag");
  uint32_t grid_shader_pipeline_id = res_create_shader_pipeline(&resource_manager, "../src/shaders/grid.vert", "../src/shaders/grid.frag");
  uint32_t font_id = res_create_font(&resource_manager, "../resources/fonts/Tamzen8x16b.ttf", 16);
  uint32_t text_mesh_id = res_create_text_mesh(&resource_manager);

  bool is_created = (base_shader_pipeline_id != INVALID_RESOURCE) & (text_shader_pipeline_id != INVALID_RESOURCE)
                    & (grid_shader_pipeline_id != INVALID_RESOURCE) & (font_id != INVALID_RESOURCE) & (text_mesh_id != INVALID_RESOURCE);

  if (!is_created) {
    printf("[ERROR] Failed to create the resources\n");
    glfwTerminate();
    return 1;
  }

  vec3 grid_position = {0.0f, -5.0f, 0.0f};
  vec2 grid_size = {100.0f, 100.0f};
  ivec2 grid_resolution = {100, 100};
  grid_initialize(&grid, &resource_manager, grid_position, grid_size, grid_resolution);
  camera_initialize(&camera);

  res_create_sphere(&resource_manager, "../resources/textures/earth.jpg",
                    (SphereProperties){
                      .position = {0.0f, 0.0f, 0.0f},
                      .velocity = {0.0f, 0.0f, 0.0f},
                      .weight = 5.972e24f,
                      .radius = 6371000.0f,
                      .sectors = 72,
                      .stacks = 24,
                    });

  res_create_sphere(&resource_manager, "../resources/textures/mercury.jpg",
                    (SphereProperties){
                      .position = {0.0f, 0.0f, 384400000.0f},
                      .velocity = {1018.289f, 0.0f, 0.0f},
                      .weight = 7.35e22f,
                      .radius = 1737500.0f,
                      .sectors = 72,
                      .stacks = 24,
                    });

  while (!glfwWindowShouldClose(window)) {
    if (!time_check_fps(&time)) continue;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    physics_apply_gravity(resource_manager.spheres.buf, resource_manager.spheres.len, ctx.time_scale, time.deltatime);
    physics_move_spheres(resource_manager.spheres.buf, resource_manager.spheres.len, ctx.time_scale, time.deltatime);
    physics_calculate_grid(&grid, resource_manager.spheres.buf, resource_manager.spheres.len, ctx.world_scale);
    grid_update_vertices(&grid, &resource_manager);
    renderer_render_scene(&ctx, base_shader_pipeline_id, grid_shader_pipeline_id);
    hud_render(&ctx, (TextResources){
                       .font_id = font_id,
                       .mesh_id = text_mesh_id,
                       .shader_pipeline_id = text_shader_pipeline_id,
                     });

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  res_remove_all(&resource_manager);
  glfwTerminate();
}

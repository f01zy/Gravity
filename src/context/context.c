#include "context/context.h"
#include "core/defines.h"
#include "core/time.h"
#include "core/utility.h"
#include "grid/grid.h"
#include "renderer/camera.h"
#include "resources/resource_manager.h"

bool context_initialize(Context *ctx, int argc, char **argv) {
  size_t arguments = argc - 1;
  if (!arguments || arguments % 8) {
    utility_help_message();
    return false;
  }

  *ctx = (Context){
    .resource_manager = calloc(1, sizeof(ResourceManager)),
    .camera = calloc(1, sizeof(Camera)),
    .grid = calloc(1, sizeof(Grid)),
    .time = calloc(1, sizeof(Time)),
    .time_scale = TIME_SCALE,
    .world_scale = WORLD_SCALE,
    .is_hud = true,
  };

  ctx->base_shader_pipeline_id = res_create_shader_pipeline(ctx->resource_manager, "../src/shaders/base.vert", "../src/shaders/base.frag");
  ctx->text_shader_pipeline_id = res_create_shader_pipeline(ctx->resource_manager, "../src/shaders/text.vert", "../src/shaders/text.frag");
  ctx->grid_shader_pipeline_id = res_create_shader_pipeline(ctx->resource_manager, "../src/shaders/grid.vert", "../src/shaders/grid.frag");
  ctx->font_id = res_create_font(ctx->resource_manager, "../resources/fonts/Tamzen8x16b.ttf", 16);
  ctx->text_mesh_id = res_create_text_mesh(ctx->resource_manager);

  bool is_created = (ctx->base_shader_pipeline_id != INVALID_RESOURCE) & (ctx->text_shader_pipeline_id != INVALID_RESOURCE)
                    & (ctx->grid_shader_pipeline_id != INVALID_RESOURCE) & (ctx->font_id != INVALID_RESOURCE) & (ctx->text_mesh_id != INVALID_RESOURCE);

  if (!is_created) {
    printf("[ERROR] Failed to create the resources\n");
    glfwTerminate();
    return false;
  }

  vec3 grid_position = {0.0f, -5.0f, 0.0f};
  vec2 grid_size = {1000.0f, 1000.0f};
  ivec2 grid_resolution = {500, 500};
  grid_initialize(ctx->grid, ctx->resource_manager, grid_position, grid_size, grid_resolution);
  camera_initialize(ctx->camera);

  size_t spheres = (argc - 1) / 8;
  for (int i = 0; i < spheres; i++) {
    double res[8];
    for (int j = 0; j < 8; j++) {
      char *status;
      res[j] = strtod(argv[i * 8 + j + 1], &status);
      if (*status != '\0') {
        utility_help_message();
        return false;
      }
    }
    res_create_sphere(ctx->resource_manager, NULL,
                      (SphereProperties){
                        .position = {res[0], res[1], res[2]},
                        .velocity = {res[3], res[4], res[5]},
                        .mass = res[6],
                        .radius = res[7],
                        .sectors = 72,
                        .stacks = 24,
                      });
  }

  return true;
}

void context_remove(const Context *ctx) {
  free(ctx->resource_manager);
  free(ctx->camera);
  free(ctx->grid);
  free(ctx->time);
}

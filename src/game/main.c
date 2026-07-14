#include <cglm/cglm.h>
#include <stdint.h>
#include <stdio.h>

#include "core/defines.h"
#include "core/graphics.h"
#include "core/time.h"
#include "game/input.h"
#include "physics/physics.h"
#include "renderer/camera.h"
#include "renderer/context.h"
#include "renderer/hud.h"
#include "renderer/renderer.h"
#include "resources/resource_manager.h"

int main() {
  if (!glfwInit()) {
    printf("[ERROR] Failed to initialize GLFW\n");
    return 1;
  }
  ResourceManager resource_manager = {0};
  Camera camera = {0};
  Time time = {.fps = FPS};
  Context ctx = {
    .resource_manager = &resource_manager,
    .camera = &camera,
    .time = &time,
    .window_size = {WIDTH, HEIGHT},
  };
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  GLFWwindow *window = glfwCreateWindow(ctx.window_size[0], ctx.window_size[1], TITLE, NULL, NULL);
  if (!window) {
    printf("[ERROR] Failed to create window\n");
    glfwTerminate();
    return 1;
  }
  camera_initialize(&camera);
  glfwSetWindowUserPointer(window, &ctx);
  glfwMakeContextCurrent(window);
  glfwSetScrollCallback(window, mouse_scroll_callback);
  glfwSetCursorPosCallback(window, mouse_position_callback);

  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    printf("[ERROR] Failed to initialize OpenGL context\n");
    glfwTerminate();
    return 1;
  }
  printf("[LOG] Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  uint32_t base_shader_pipeline_id = res_create_shader_pipeline(&resource_manager, "../src/shaders/base.vert", "../src/shaders/base.frag");
  uint32_t text_shader_pipeline_id = res_create_shader_pipeline(&resource_manager, "../src/shaders/text.vert", "../src/shaders/text.frag");
  uint32_t font_id = res_create_font(&resource_manager, "../resources/fonts/Tamzen8x16b.ttf", 16);
  uint32_t text_mesh_id = res_create_text_mesh(&resource_manager);

  if (base_shader_pipeline_id == INVALID_RESOURCE || text_shader_pipeline_id == INVALID_RESOURCE || font_id == INVALID_RESOURCE
      || text_mesh_id == INVALID_RESOURCE) {
    printf("[ERROR] Failed to create the resources\n");
    glfwTerminate();
    return 1;
  }

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

    physics_apply_gravity(resource_manager.spheres.buf, resource_manager.spheres.len, time.deltatime);
    physics_move_spheres(resource_manager.spheres.buf, resource_manager.spheres.len, time.deltatime);
    renderer_render_scene(&ctx, base_shader_pipeline_id);
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

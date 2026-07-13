#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdint.h>
#include <stdio.h>

#include "core/defines.h"
#include "core/time.h"
#include "physics/physics.h"
#include "renderer/camera.h"
#include "renderer/hud.h"
#include "renderer/renderer.h"
#include "renderer/text.h"
#include "resources/resource_manager.h"

Camera camera;
float last_mouse_x;
float last_mouse_y;
bool is_first_mouse = true;

void handle_keyboard(GLFWwindow *window, float deltatime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  float radius = camera.radius - yoffset;
  if (radius >= 0.0f && radius <= MAX_CAMERA_RADIUS) camera.radius = radius;
}

void mouse_position_callback(GLFWwindow *window, double xpos, double ypos) {
  if (is_first_mouse) {
    last_mouse_x = xpos;
    last_mouse_y = ypos;
    is_first_mouse = false;
  }
  float xoffset = xpos - last_mouse_x;
  float yoffset = ypos - last_mouse_y;
  last_mouse_x = xpos;
  last_mouse_y = ypos;
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    camera.pitch = glm_clamp(camera.pitch + yoffset, -89.0f, 89.0f);
    camera.yaw += xoffset;
  }
}

int main() {
  if (!glfwInit()) {
    printf("[ERROR] Failed to initialize GLFW\n");
    return 1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
  if (!window) {
    printf("[ERROR] Failed to create window\n");
    glfwTerminate();
    return 1;
  }
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

  ResourceManager resource_manager = {0};
  Time time = {0};
  time.fps = FPS;
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

  camera_initialize(&camera);
  res_create_sphere(&resource_manager, "../resources/textures/earth.jpg",
                    (SphereProperties){
                      .position = {0.0f, 0.0f, 0.0f},
                      .velocity = {0.0f, 0.0f, 0.0f},
                      .weight = 3000000000000.0f,
                      .radius = 10.0f,
                      .sectors = 72,
                      .stacks = 24,
                    });

  res_create_sphere(&resource_manager, "../resources/textures/mercury.jpg",
                    (SphereProperties){
                      .position = {0.0f, 0.0f, 30.0f},
                      .velocity = {2.583f, 0.0f, 0.0f},
                      .weight = 1000000.0f,
                      .radius = 3.0f,
                      .sectors = 72,
                      .stacks = 24,
                    });

  while (!glfwWindowShouldClose(window)) {
    if (!time_check_fps(&time)) continue;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    handle_keyboard(window, time.deltatime);
    physics_apply_gravity(resource_manager.spheres.buf, resource_manager.spheres.len, time.deltatime);
    physics_move_spheres(resource_manager.spheres.buf, resource_manager.spheres.len, time.deltatime);
    renderer_render_scene(&resource_manager, &camera, base_shader_pipeline_id);
    hud_render(&resource_manager, (TextResources){.font_id = font_id, .mesh_id = text_mesh_id, .shader_pipeline_id = text_shader_pipeline_id}, time.deltatime);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  res_remove_all(&resource_manager);
  glfwTerminate();
}

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdint.h>
#include <stdio.h>

#include "camera.h"
#include "defines.h"
#include "physics.h"
#include "renderer.h"
#include "resource_manager.h"

Camera camera;
float last_mouse_x;
float last_mouse_y;
float last_frame = 0.0f;
float deltatime = 0.0f;
vec3 cubes[] = {{0.0f, 0.0f, 0.0f}};
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
  camera_initialize(&camera);
  ResourceManager resource_manager = {0};
  uint32_t shader_pipeline_id = res_create_shader_pipeline(&resource_manager, "../src/shaders/vertex.glsl", "../src/shaders/fragment.glsl");
  if (shader_pipeline_id == INVALID_RESOURCE) {
    printf("[ERROR] Failed to create resources\n");
    glfwTerminate();
    return 1;
  }

  vec3 position1 = {0.0f, 0.0f, 0.0f};
  vec3 velocity1 = {0.0f, 0.0f, 0.0f};
  res_create_sphere(&resource_manager, position1, velocity1, 3000000000000.0f, 10.0f, 72, 24, "../resources/textures/earth.jpg");

  vec3 position2 = {0.0f, 0.0f, 30.0f};
  vec3 velocity2 = {2.583f, 0.0f, 0.0f};
  res_create_sphere(&resource_manager, position2, velocity2, 1000000.0f, 3.0f, 72, 24, "../resources/textures/mercury.jpg");

  vec3 position3 = {50.0f, 0.0f, 0.0f};
  vec3 velocity3 = {0.0f, 0.0f, 2.583f};
  res_create_sphere(&resource_manager, position3, velocity3, 1000000.0f, 3.0f, 72, 24, "../resources/textures/mercury.jpg");

  while (!glfwWindowShouldClose(window)) {
    float now = glfwGetTime();
    float need = 1.0f / FPS;
    deltatime = now - last_frame;
    if (deltatime < need) continue;
    last_frame = now;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    handle_keyboard(window, deltatime);
    physics_apply_gravity(&resource_manager, deltatime);
    physics_move_spheres(&resource_manager, deltatime);
    render_scene(&resource_manager, &camera, shader_pipeline_id);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  res_remove_all(&resource_manager);
  glfwTerminate();
}

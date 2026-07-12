#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdint.h>
#include <stdio.h>

#include "camera.h"
#include "defines.h"
#include "renderer.h"
#include "resource_manager.h"
#include "shader.h"

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
  vec3 temp = {0.0f, 0.0f, 0.0f};
  ResourceManager resource_manager = {0};
  uint32_t shader_pipeline_id = res_create_shader_pipeline(&resource_manager, "../src/shaders/vertex.glsl", "../src/shaders/fragment.glsl");
  uint32_t sphere_id = res_create_sphere(&resource_manager, temp, temp, 1.0f, 3.0f, 72, 24, "../resources/textures/earth.jpg");
  if (shader_pipeline_id == INVALID_RESOURCE || sphere_id == INVALID_RESOURCE) {
    printf("[ERROR] Failed to create resources\n");
    glfwTerminate();
    return 1;
  }
  ShaderPipeline *shader_pipeline = res_get_shader_pipeline(&resource_manager, shader_pipeline_id);

  while (!glfwWindowShouldClose(window)) {
    float now = glfwGetTime();
    float need = 1.0f / FPS;
    deltatime = now - last_frame;
    if (deltatime < need) continue;
    last_frame = now;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    handle_keyboard(window, deltatime);

    glUseProgram(shader_pipeline->shader_program);
    mat4 view;
    mat4 projection;
    update_position(&camera);
    get_view_matrix(&camera, view);
    glm_perspective(camera.fov, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f, projection);
    uniform_set_mat4(shader_pipeline->shader_program, "view", view);
    uniform_set_mat4(shader_pipeline->shader_program, "projection", projection);
    render_sphere(&resource_manager, sphere_id, shader_pipeline_id);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();
}

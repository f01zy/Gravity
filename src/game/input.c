#include "game/input.h"
#include "core/defines.h"
#include "core/graphics.h"
#include "renderer/context.h"

float last_mouse_x;
float last_mouse_y;
bool is_first_mouse = true;

void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  Context *ctx = (Context *)glfwGetWindowUserPointer(window);
  float radius = ctx->camera->radius - yoffset;
  if (radius >= 0.0f && radius <= MAX_CAMERA_RADIUS) ctx->camera->radius = radius;
}

void mouse_position_callback(GLFWwindow *window, double xpos, double ypos) {
  Context *ctx = (Context *)glfwGetWindowUserPointer(window);
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
    ctx->camera->pitch = glm_clamp(ctx->camera->pitch + yoffset, -89.0f, 89.0f);
    ctx->camera->yaw += xoffset;
  }
}

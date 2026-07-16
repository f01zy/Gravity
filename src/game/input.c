#include <cglm/cglm.h>

#include "context/context.h"
#include "core/defines.h"
#include "core/graphics.h"
#include "core/time.h"
#include "game/input.h"

float last_mouse_x;
float last_mouse_y;
bool is_first_mouse = true;

void input_mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  Context *ctx = (Context *)glfwGetWindowUserPointer(window);
  float radius = ctx->camera->radius - yoffset;
  if (radius >= 0.0f && radius <= MAX_CAMERA_RADIUS) ctx->camera->radius = radius;
}

void input_mouse_position_callback(GLFWwindow *window, double xpos, double ypos) {
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

void input_handle_keyboard(GLFWwindow *window) {
  Context *ctx = (Context *)glfwGetWindowUserPointer(window);
  if (!time_check_press(ctx->time)) return;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwTerminate();
  if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) ctx->is_hud = !ctx->is_hud;
  if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) ctx->world_scale *= WORLD_SCALE_COEFFICIENT;
  if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) ctx->world_scale /= WORLD_SCALE_COEFFICIENT;
  if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) ctx->time_scale = glm_clamp(ctx->time_scale -= TIME_SCALE_SPEED, 1, MAX_TIME_SCALE);
  if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) ctx->time_scale = glm_clamp(ctx->time_scale += TIME_SCALE_SPEED, 1, MAX_TIME_SCALE);
}

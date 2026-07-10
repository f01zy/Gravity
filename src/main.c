#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdio.h>

#include "camera.h"
#include "defines.h"
#include "shader.h"
#include "vertices.h"

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
    printf("ERROR: Failed to initialize GLFW\n");
    return 1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
  if (!window) {
    printf("ERROR: Failed to create window\n");
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);
  glfwSetScrollCallback(window, mouse_scroll_callback);
  glfwSetCursorPosCallback(window, mouse_position_callback);
  glEnable(GL_DEPTH_TEST);
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    printf("ERROR: Failed to initialize GLEW: %s\n", glewGetErrorString(err));
    glfwTerminate();
    return 1;
  }

  unsigned VAO, VBO, IBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &IBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(sizeof(float) * 3));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  unsigned shader_program = create_shader_program("/home/f01zy/Programming/Gravity/src/vertex.glsl", "/home/f01zy/Programming/Gravity/src/fragment.glsl");
  initialize_camera(&camera);

  while (!glfwWindowShouldClose(window)) {
    float now = glfwGetTime();
    float need = 1.0f / FPS;
    deltatime = now - last_frame;
    if (deltatime < need) continue;
    last_frame = now;
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    handle_keyboard(window, deltatime);

    glUseProgram(shader_program);
    mat4 view;
    mat4 projection;
    update_position(&camera);
    get_view_matrix(&camera, view);
    glm_perspective(camera.fov, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f, projection);
    set_mat4(shader_program, "view", view);
    set_mat4(shader_program, "projection", projection);

    glBindVertexArray(VAO);
    for (int i = 0; i < sizeof(cubes) / sizeof(cubes[0]); i++) {
      mat4 model = GLM_MAT4_IDENTITY_INIT;
      glm_translate(model, cubes[i]);
      set_mat4(shader_program, "model", model);
      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &IBO);
  glfwTerminate();
}

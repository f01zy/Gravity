#include <stdbool.h>

#include "GLFW/glfw3.h"
#include "time.h"

bool time_check_fps(Time *time) {
  float now = glfwGetTime();
  float need = 1.0f / time->fps;
  float deltatime = now - time->lastframe;
  if (deltatime < need) return false;
  time->lastframe = now;
  time->deltatime = deltatime;
  return true;
}

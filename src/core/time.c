#include <stdbool.h>

#include "core/defines.h"
#include "core/graphics.h"
#include "time.h"

bool time_check_fps(Time *time) {
  float now = glfwGetTime();
  float need = 1.0f / FPS;
  float deltatime = now - time->lastframe;
  if (deltatime < need) return false;
  time->lastframe = now;
  time->deltatime = deltatime;
  return true;
}

bool time_check_press(Time *time) {
  float now = glfwGetTime();
  float deltatime = (now - time->lastpress) * 1000;
  if (deltatime < PRESS_TIMER) return false;
  time->lastpress = now;
  return true;
}

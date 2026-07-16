#ifndef TIME_INCLUDED
#define TIME_INCLUDED

#include <stdbool.h>

typedef struct {
  float lastframe;
  float lastpress;
  float deltatime;
} Time;

bool time_check_fps(Time *time);
bool time_check_press(Time *time);

#endif

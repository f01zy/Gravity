#ifndef TIME_INCLUDED
#define TIME_INCLUDED

#include <stdbool.h>

typedef struct {
  float lastframe;
  float deltatime;
  float fps;
} Time;

bool time_check_fps(Time *time);

#endif

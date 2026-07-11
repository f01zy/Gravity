#ifndef DEFINES_INCLUDED
#define DEFINES_INCLUDED

#define TITLE              "Gravity simulation"
#define WIDTH              900
#define HEIGHT             600
#define FPS                60
#define PI                 3.1415927f

#define MAX_VERTICES_COUNT 4000
#define MAX_INDICES_COUNT  8000
#define MAX_CAMERA_RADIUS  40.0f
#define MAX_FILE_LEN       8192

#define CLAMP(A, B, C)     ((A) < (B) ? (B) : (A) > (C) ? (C) : (A))

#endif

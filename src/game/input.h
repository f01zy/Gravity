#ifndef INPUT_INCLUDED
#define INPUT_INCLUDED

#include "core/graphics.h"

void input_mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void input_mouse_position_callback(GLFWwindow *window, double xpos, double ypos);
void input_handle_keyboard(GLFWwindow *window);

#endif

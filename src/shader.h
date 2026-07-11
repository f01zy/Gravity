#ifndef SHADER_INCLUDED
#define SHADER_INCLUDED

#include <cglm/cglm.h>

unsigned create_shader_program(const char *vertex_shader_source, const char *fragment_shader_source);
void set_mat4(unsigned shader_program, const char *name, mat4 mat);
void set_int(unsigned shader_program, const char *name, int x);

#endif

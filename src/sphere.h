#ifndef SPHERE_INCLUDED
#define SPHERE_INCLUDED

#include <cglm/cglm.h>
#include <stddef.h>

#include "defines.h"
#include "mesh.h"

typedef struct {
  vec3 position;
  vec3 velocity;
  float weight;
  float radius;
  int sectors;
  int stacks;
  unsigned texture;

  struct {
    Vertice buf[MAX_VERTICES_COUNT];
    size_t len;
  } vertices;

  struct {
    ivec3 buf[MAX_INDICES_COUNT];
    size_t len;
  } indices;
} Sphere;

void initialize_sphere(Sphere *sphere, vec3 position, vec3 velocity, float weight, float radius, int sectors, int stacks, const char *texture_path);
void render_sphere(Sphere *sphere, const Mesh *mesh, unsigned shader_program);
size_t get_sphere_vertices_size(const Sphere *sphere);
size_t get_sphere_indices_size(const Sphere *sphere);

#endif

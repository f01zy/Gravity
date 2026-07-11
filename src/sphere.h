#ifndef SPHERE_INCLUDED
#define SPHERE_INCLUDED

#include <cglm/cglm.h>
#include <stddef.h>

#include "defines.h"
#include "mesh.h"

typedef struct {
  float radius;
  int sectors;
  int stacks;

  struct {
    Vertice buf[MAX_VERTICES_COUNT];
    size_t len;
  } vertices;

  struct {
    ivec3 buf[MAX_INDICES_COUNT];
    size_t len;
  } indices;
} Sphere;

void initialize_sphere(Sphere *sphere, float radius, int sectors, int stacks);
void render_sphere(const Sphere *sphere, const Mesh *mesh);
void sphere_debug(const Sphere *sphere);
size_t get_sphere_vertices_size(const Sphere *sphere);
size_t get_sphere_indices_size(const Sphere *sphere);

#endif

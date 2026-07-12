#ifndef SPHERE_INCLUDED
#define SPHERE_INCLUDED

#include <cglm/cglm.h>
#include <stddef.h>
#include <stdint.h>

#include "defines.h"
#include "mesh.h"

typedef struct {
  Vertice buf[MAX_VERTICES_COUNT];
  size_t len;
} Vertices;

typedef struct {
  ivec3 buf[MAX_INDICES_COUNT];
  size_t len;
} Indices;

typedef struct {
  vec3 position;
  vec3 velocity;
  float weight;
  float radius;
  int sectors;
  int stacks;
  uint32_t texture;
  uint32_t mesh;
  Vertices vertices;
  Indices indices;
} Sphere;

typedef enum {
  SPHERE_INIT_MISSING_DATA,
  SPHERE_INIT_SUCCESS,
} SphereInitStatus;

SphereInitStatus sphere_initialize(Sphere *sphere, vec3 position, vec3 velocity, float weight, float radius, int sectors, int stacks);
size_t get_sphere_vertices_size(const Sphere *sphere);
size_t get_sphere_indices_size(const Sphere *sphere);

#endif

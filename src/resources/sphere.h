#ifndef SPHERE_INCLUDED
#define SPHERE_INCLUDED

#include <cglm/cglm.h>
#include <stddef.h>
#include <stdint.h>

#include "mesh.h"

typedef struct {
  Vertice *buf;
  size_t len;
  size_t size;
} Vertices;

typedef struct {
  ivec3 *buf;
  size_t len;
  size_t size;
} Indices;

typedef struct {
  vec3 position;
  vec3 velocity;
  float weight;
  float radius;
  int sectors;
  int stacks;
  uint32_t texture_id;
  uint32_t mesh_id;
  Vertices vertices;
  Indices indices;
} Sphere;

typedef struct {
  vec3 position;
  vec3 velocity;
  float weight;
  float radius;
  int sectors;
  int stacks;
} SphereProperties;

typedef enum {
  SPHERE_INIT_MISSING_DATA,
  SPHERE_INIT_SUCCESS,
} SphereInitStatus;

SphereInitStatus sphere_initialize(Sphere *sphere, SphereProperties properties);
void sphere_remove(Sphere *sphere);
size_t get_sphere_vertices_size(const Sphere *sphere);
size_t get_sphere_indices_size(const Sphere *sphere);

#endif

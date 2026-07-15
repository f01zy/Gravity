#ifndef PHYSICS_INCLUDED
#define PHYSICS_INCLUDED

#include <stddef.h>

#include "grid/grid.h"
#include "resources/sphere.h"

void physics_calculate_grid(Grid *grid, Sphere *spheres, size_t len, float world_scale);
void physics_apply_gravity(Sphere *spheres, size_t len, float time_scale, float deltatime);
void physics_move_spheres(Sphere *spheres, size_t len, float time_scale, float deltatime);

#endif

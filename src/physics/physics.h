#ifndef PHYSICS_INCLUDED
#define PHYSICS_INCLUDED

#include "resources/sphere.h"
#include <stddef.h>

void physics_apply_gravity(Sphere *spheres, size_t len, float deltatime);
void physics_move_spheres(Sphere *spheres, size_t len, float deltatime);

#endif

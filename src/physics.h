#ifndef PHYSICS_INCLUDED
#define PHYSICS_INCLUDED

#include "resource_manager.h"

void physics_apply_gravity(ResourceManager *resource_manager, float deltatime);
void physics_move_spheres(const ResourceManager *resource_manager, float deltatime);

#endif

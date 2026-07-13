#ifndef HUD_INCLUDED
#define HUD_INCLUDED

#include "renderer/text.h"
#include "resources/resource_manager.h"

void hud_render(const ResourceManager *resource_manager, TextResources resources, float deltatime);

#endif

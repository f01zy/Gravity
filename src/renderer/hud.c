#include <stddef.h>
#include <stdio.h>

#include "renderer/hud.h"
#include "renderer/text.h"
#include "resources/resource_manager.h"

void hud_render(const ResourceManager *resource_manager, TextResources resources, float deltatime) {
  float fps = 1.0f / deltatime;
  char buf[64];
  size_t len = snprintf(buf, sizeof(buf) - 1, "%.0f FPS", fps);
  buf[len] = '\0';
  // TODO: передать размеры экрана извне
  render_text(resource_manager, resources,
              (TextProperties){
                .text = buf,
                .position = {20.0f, 570.0f},
                .scale = 1.0f,
                .color = {1.0f, 1.0f, 1.0f},
              });
}

#include <stddef.h>
#include <stdio.h>

#include "core/time.h"
#include "renderer/context.h"
#include "renderer/hud.h"
#include "renderer/text.h"

void hud_render(const Context *ctx, TextResources resources) {
  float fps = 1.0f / ctx->time->deltatime;
  char buf[64];
  buf[snprintf(buf, sizeof(buf) - 1, "%.0f FPS", fps)] = '\0';
  render_text(ctx, resources,
              (TextProperties){
                .text = buf,
                .position = {20.0f, ctx->window_size[1] - 30.0f},
                .scale = 1.0f,
                .color = {1.0f, 1.0f, 1.0f},
              });
}

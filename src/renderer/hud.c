#include <stddef.h>
#include <stdio.h>

#include "context/context.h"
#include "core/defines.h"
#include "core/time.h"
#include "renderer/hud.h"
#include "renderer/text.h"

void hud_render(const Context *ctx, TextResources resources) {
  float fps = 1.0f / ctx->time->deltatime;
  char buf[128];
  buf[snprintf(buf, sizeof(buf) - 1, "FPS: %.0f", fps)] = '\0';
  render_text(ctx, resources,
              (TextProperties){
                .text = buf,
                .position = {20.0f, HEIGHT - 30.0f},
                .scale = 1.0f,
                .color = {1.0f, 1.0f, 1.0f},
              });

  buf[snprintf(buf, sizeof(buf) - 1, "World scale: 1:%.0f", 1.0f / ctx->world_scale)] = '\0';
  render_text(ctx, resources,
              (TextProperties){
                .text = buf,
                .position = {20.0f, HEIGHT - 50.0f},
                .scale = 1.0f,
                .color = {1.0f, 1.0f, 1.0f},
              });

  buf[snprintf(buf, sizeof(buf) - 1, "Time scale: %.0f", ctx->time_scale)] = '\0';
  render_text(ctx, resources,
              (TextProperties){
                .text = buf,
                .position = {20.0f, HEIGHT - 70.0f},
                .scale = 1.0f,
                .color = {1.0f, 1.0f, 1.0f},
              });
}

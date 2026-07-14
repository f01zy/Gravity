#include <stddef.h>
#include <stdio.h>

#include "core/time.h"
#include "renderer/context.h"
#include "renderer/hud.h"
#include "renderer/text.h"

void hud_render(const Context *ctx, TextResources resources) {
  float fps = 1.0f / ctx->time->deltatime;
  char fps_buf[64];
  fps_buf[snprintf(fps_buf, sizeof(fps_buf) - 1, "%.0f FPS", fps)] = '\0';

  vec3 *position = &ctx->camera->position;
  char position_buf[64];
  position_buf[snprintf(position_buf, sizeof(position_buf) - 1, "%f, %f, %f", *position[0], *position[1], *position[2])] = '\0';

  render_text(ctx, resources,
              (TextProperties){
                .text = fps_buf,
                .position = {20.0f, ctx->window_size[1] - 30.0f},
                .scale = 1.0f,
                .color = {1.0f, 1.0f, 1.0f},
              });

  render_text(ctx, resources,
              (TextProperties){
                .text = position_buf,
                .position = {20.0f, ctx->window_size[1] - 60.0f},
                .scale = 1.0f,
                .color = {1.0f, 1.0f, 1.0f},
              });
}

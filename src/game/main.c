#include <cglm/cglm.h>
#include <stdbool.h>
#include <stddef.h>

#include "context/context.h"
#include "game/game.h"
#include "game/window.h"

int main(int argc, char **argv) {
  Context ctx;
  GLFWwindow *window = window_create(&ctx);
  if (!window) return 1;
  if (!context_initialize(&ctx, argc, argv)) return 1;
  game_loop(window, &ctx);
}

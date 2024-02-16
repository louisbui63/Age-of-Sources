#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

int main(int argc, char **argv) {
  HANDLE_ERROR(SDL_Init(SDL_INIT_VIDEO), abort());
  SDL_Window *window =
      SDL_CreateWindow("test", 100, 100, 420, 360, SDL_WINDOW_SHOWN);
  HANDLE_NULLPTR(window, abort());
  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  HANDLE_NULLPTR(renderer, {
    SDL_DestroyWindow(window);
    abort();
  });

  // elapsed is the frametime from last frame
  int elapsed = TARGET_FRAMETIME;
  for (;;) {
    int start_time = SDL_GetTicks();

    elapsed = max(TARGET_FRAMETIME, SDL_GetTicks() + start_time);
    SDL_Delay(TARGET_FRAMETIME - elapsed);
  }
}

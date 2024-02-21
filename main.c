#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>

#include "util.h"

int main(int argc, char **argv) {
  HANDLE_ERROR(SDL_Init(SDL_INIT_VIDEO) < 0, SDL_GetError(), abort());
  SDL_Window *window =
      SDL_CreateWindow("test", 100, 100, 420, 360, SDL_WINDOW_SHOWN);
  HANDLE_ERROR(!window, SDL_GetError(), abort());
  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  HANDLE_ERROR(!renderer, SDL_GetError(), {
    SDL_DestroyWindow(window);
    abort();
  });

  SDL_Surface *test_bmp = SDL_LoadBMP("test.bmp");
  HANDLE_ERROR(!test_bmp, SDL_GetError(), {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    abort();
  })
  SDL_Texture *test_tex = SDL_CreateTextureFromSurface(renderer, test_bmp);
  HANDLE_ERROR(!test_tex, SDL_GetError(), {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    abort();
  })

  // dt is the frametime from last frame
  int dt = TARGET_FRAMETIME;
  for (;;) {
    int start_time = SDL_GetTicks();

    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, test_tex, 0, 0);

    SDL_RenderPresent(renderer);

    dt = max(TARGET_FRAMETIME, SDL_GetTicks() + start_time);
    SDL_Delay(TARGET_FRAMETIME - dt);
  }
}

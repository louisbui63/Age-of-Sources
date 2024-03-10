#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>

#include "camera.h"
#include "components.h"
#include "ecs.h"
#include "sprite.h"
#include "util.h"

int main() {
  HANDLE_ERROR(SDL_Init(SDL_INIT_VIDEO) < 0, SDL_GetError(), abort());
  atexit(SDL_Quit);

  SDL_Window *window =
      SDL_CreateWindow("test", 100, 100, 640, 360, SDL_WINDOW_SHOWN);

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
  SDL_FreeSurface(test_bmp);

  World w = world_new();
  Camera cam = {.x = 100, .y = 100, .zoom = 1};

  init_world(&w);

  Entity *test_e = spawn_entity(&w);

  Position *test_pos = malloc(sizeof(Position));
  *test_pos = (Position){.x = 155, .y = 250};
  ecs_add_component(&w, test_e, COMP_POSITION, test_pos);

  Sprite *test_sprite = malloc(sizeof(Sprite));
  SDL_Rect *size = malloc(sizeof(SDL_Rect));
  SDL_QueryTexture(test_tex, 0, 0, &size->w, &size->h);
  size->x = 0;
  size->y = 0;

  *test_sprite = (Sprite){.texture = test_tex, .rect = size};
  ecs_add_component(&w, test_e, COMP_SPRITE, test_sprite);

  // dt is the frametime from last frame
  int dt = TARGET_FRAMETIME;
  for (;;) {
    int start_time = SDL_GetTicks64();

    SDL_RenderClear(renderer);
    render(&w, renderer, &cam);

    SDL_RenderPresent(renderer);

    dt = max(TARGET_FRAMETIME, SDL_GetTicks64() - start_time);
    SDL_Delay(TARGET_FRAMETIME - dt);
  }

  world_free(&w);
}

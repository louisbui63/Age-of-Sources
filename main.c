#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include "asset_manager.h"
#include "camera.h"
#include "components.h"
#include "ecs.h"
#include "input.h"
#include "sprite.h"
#include "util.h"

int main() {
  init_asset_manager();

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
  Uint32 dt = TARGET_FRAMETIME;
  SDL_Event event;

  // down keys and mouse buttons
  Inputs *input_down = inputs_new();

  int running = 1;
  for (; running;) {
    Uint32 start_time = SDL_GetTicks();

    Inputs *input_pressed = inputs_new();
    Inputs *input_released = inputs_new();

    // SDL events
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
        if (!inputs_is_key_in(input_down, event.key.keysym.sym)) {
          inputs_update_key_in(input_pressed, event.key.keysym.sym, true);
          inputs_update_key_in(input_down, event.key.keysym.sym, true);
        }
        break;
      case SDL_KEYUP:
        inputs_update_key_in(input_down, event.key.keysym.sym, false);
        inputs_update_key_in(input_released, event.key.keysym.sym, true);
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (!inputs_is_mouse_button_in(input_down, event.button.button)) {
          inputs_update_mouse_button_in(input_down, event.button.button, true);
          inputs_update_mouse_button_in(input_pressed, event.button.button,
                                        true);
        }
        break;
      case SDL_MOUSEBUTTONUP:
        inputs_update_mouse_button_in(input_down, event.button.button, false);
        inputs_update_mouse_button_in(input_released, event.button.button,
                                      true);
        break;
      case SDL_QUIT:
        running = 0;
        break;
      }
    }
    // keyboard and mouse button events
    inputs_run_callbacks(&w, input_pressed, KEY_PRESSED);
    inputs_run_callbacks(&w, input_down, KEY_DOWN);
    inputs_run_callbacks(&w, input_released, KEY_RELEASED);

    // free instant inputs
    inputs_free(input_pressed);
    inputs_free(input_released);

    // render
    SDL_RenderClear(renderer);
    render(&w, renderer, &cam);

    SDL_RenderPresent(renderer);

    // delay before next frame
    dt = min(TARGET_FRAMETIME, SDL_GetTicks() - start_time);
    if (running && dt != TARGET_FRAMETIME)
      SDL_Delay(TARGET_FRAMETIME - dt);
  }

  inputs_free(input_down);
  world_free(&w);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

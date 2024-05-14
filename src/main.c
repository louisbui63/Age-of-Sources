#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>

#include "./renderer/button.h"
#include "components.h"
#include "data_structures/asset_manager.h"
#include "data_structures/ecs.h"
#include "data_structures/map.h"
#include "input.h"
#include "renderer/camera.h"
#include "renderer/sprite.h"
#include "renderer/ui.h"
#include "util.h"

#include "audio/audio.h"

int RUNNING = 1;

int main() {
  init_asset_manager();

  HANDLE_ERROR(SDL_Init(SDL_INIT_VIDEO) < 0, SDL_GetError(), abort());
  atexit(SDL_Quit);
  HANDLE_ERROR(TTF_Init() < 0, SDL_GetError(), abort());
  atexit(TTF_Quit);
  SDL_Window *window = SDL_CreateWindow(
      "test", 100, 100, WIN_W, WIN_H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  HANDLE_ERROR(!window, SDL_GetError(), abort());
  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  HANDLE_ERROR(!renderer, SDL_GetError(), {
    SDL_DestroyWindow(window);
    abort();
  });

  HANDLE_ERROR(SDL_RenderSetLogicalSize(renderer, WIN_W, WIN_H), SDL_GetError(),
               {
                 SDL_DestroyRenderer(renderer);
                 SDL_DestroyWindow(window);
                 abort();
               });

  HANDLE_ERROR(SDL_RenderSetIntegerScale(renderer, true), SDL_GetError(), {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    abort();
  });

  HANDLE_ERROR(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0,
               Mix_GetError(), {
                 SDL_DestroyRenderer(renderer);
                 SDL_DestroyWindow(window);
                 abort();
               });

  atexit(Mix_Quit);

  SDL_Surface *test_bmp = SDL_LoadBMP("./asset/sprites/test.bmp");
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
  Camera cam = {.x = 32, .y = 32, .zoom = 1};

  init_world(&w);

  // Entity *test_e = spawn_entity(&w);

  Position *test_pos = malloc(sizeof(Position));
  *test_pos = (Position){.x = 155, .y = 250};
  // ecs_add_component(&w, test_e, COMP_POSITION, test_pos);

  spawn_main_menu(&w, renderer, window);
  // spawn_main_quit(&w, renderer, window);

  // ecs_add_component(&w, test_e, COMP_SPRITE, test_sprite);
  // ecs_add_component(&w, test_e, COMP_BACKGROUND,
  // test_background); ecs_add_component(&w, test_e,
  // COMP_CLICKABLE, test_clickable);

  // dt is the frametime from last frame
  Uint32 dt = TARGET_FRAMETIME;
  SDL_Event event;

  // down keys and mouse buttons
  Inputs *input_down = inputs_new();

  Entity *map = spawn_entity(&w);
  MapComponent *mc = malloc(sizeof(MapComponent));
  *mc = (MapComponent){load_map_from_bmp("asset/test_map.bmp")};
  ecs_add_component(&w, map, COMP_MAPCOMPONENT, mc);
  Background *back = spawn_backbackground(renderer, window);

  play_audio("asset/sfx/click.wav", 0);
  for (; RUNNING;) {
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
        RUNNING = 0;
        break;
      }
    }
    // keyboard and mouse button events
    inputs_run_callbacks(&w, renderer, input_pressed, KEY_PRESSED);
    inputs_run_callbacks(&w, renderer, input_down, KEY_DOWN);
    inputs_run_callbacks(&w, renderer, input_released, KEY_RELEASED);

    // free instant inputs
    inputs_free(input_pressed);
    inputs_free(input_released);

    // render
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, back->sprite->texture, back->sprite->rect,
                   back->rect);
    render(&w, renderer, &cam, window);
    render_ui(&w, renderer, window);

    SDL_RenderPresent(renderer);

    // delay before next frame
    dt = min(TARGET_FRAMETIME, SDL_GetTicks() - start_time);
    if (RUNNING && dt != TARGET_FRAMETIME)
      SDL_Delay(TARGET_FRAMETIME - dt);
  }

  inputs_free(input_down);
  world_free(&w);
  background_component_free(back);

  free_asset_store();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

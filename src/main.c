#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "./renderer/button.h"
#include "ai/movement.h"
#include "ai/steering_behaviors.h"
#include "components.h"
#include "data_structures/asset_manager.h"
#include "data_structures/ecs.h"
#include "data_structures/map.h"
#include "input.h"
#include "parser.h"
#include "players.h"
#include "renderer/anim.h"
#include "renderer/camera.h"
#include "renderer/sprite.h"
#include "renderer/ui.h"
#include "selection.h"
#include "units/unit_function.h"
#include "util.h"

#include "audio/audio.h"

Running RUNNING = MAIN;
char IS_FULLSCREEN = false;
HashMap GRID_FUNCTION_MAP;

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

  set_grid_functions();

  World w = world_new();
  Camera *camcam = malloc(sizeof(Camera));
  *camcam = (Camera){.x = 32, .y = 32, .zoom = 1};

  init_world(&w);

  Entity *wine = spawn_entity(&w);
  Window wind = {.w = window};
  KeyEvent *wink = malloc(sizeof(KeyEvent));
  *wink = key_event_escape;
  ecs_add_component(&w, wine, COMP_WINDOW, &wind);
  ecs_add_component(&w, wine, COMP_KEY_EVENT, wink);

  Entity *cam = spawn_entity(&w);
  KeyEvent *cammove = malloc(sizeof(KeyEvent));
  *cammove = map_movement;
  ecs_add_component(&w, cam, COMP_CAMERA, camcam);
  ecs_add_component(&w, cam, COMP_KEY_EVENT, cammove);

  // render_game_state(&w);

  {
    Entity *e = spawn_entity(&w);
    Unit *u = parse("src/units/unit_tanuki.c", renderer, window);
    ecs_add_component(&w, e, COMP_UNIT, u);
    ecs_add_component(&w, e, COMP_SPRITE, u->sprite);
    Position *p = calloc(1, sizeof(Position));
    *p = (Position){100, 100};
    ecs_add_component(&w, e, COMP_POSITION, p);
    SteerManager *stm = malloc(sizeof(SteerManager));
    *stm = (SteerManager){
        10, 10, 10, 10, 10, 0, (Vec2){0, 0}, (Vec2){100, 100}, (Vec2){0, 0}, 0};
    ecs_add_component(&w, e, COMP_STEERMANAGER, stm);
    Selectable *s = calloc(1, sizeof(Selectable));
    ecs_add_component(&w, e, COMP_SELECTABLE, s);
    Animator *a = malloc(sizeof(Animator));
    *a = animator_new(u);
    ecs_add_component(&w, e, COMP_ANIMATOR, a);
  }

  {
    Entity *e = spawn_entity(&w);
    Selector *s = malloc(sizeof(Selector));
    *s = (Selector){Normal, {0, 0}, {0, 0}, 0, vec_new(EntityRef), 0};
    ecs_add_component(&w, e, COMP_SELECTOR, s);
    KeyEvent *select_events = malloc(sizeof(KeyEvent));
    *select_events = selection_event;
    ecs_add_component(&w, e, COMP_KEY_EVENT, select_events);
  }

  for (uint i = 0; i < 2; i++) {
    Entity *e = spawn_entity(&w);
    PlayerManager *pm = malloc(sizeof(PlayerManager));
    *pm = (PlayerManager){i, 0, 0};
    ecs_add_component(&w, e, COMP_PLAYERMANAGER, pm);
  }

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
  *mc = (MapComponent){load_map_from_bmp("asset/map.bmp")};
  ecs_add_component(&w, map, COMP_MAPCOMPONENT, mc);
  Background *back = spawn_backbackground(renderer, window);

  play_audio("asset/sfx/click.wav", 0);
  for (; RUNNING != STOP;) {
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

    if (RUNNING == IN_GAME)
      move_units(&w);

    // free instant inputs
    inputs_free(input_pressed);
    inputs_free(input_released);

    // render
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, back->sprite->texture, back->sprite->rect,
                   back->rect);
    render(&w, renderer, camcam, window);
    render_ui(&w, renderer, window);
    draw_selection(&w, renderer, window);

    SDL_RenderPresent(renderer);

    // delay before next frame
    dt = min(TARGET_FRAMETIME, SDL_GetTicks() - start_time);
    if (RUNNING != STOP && dt != TARGET_FRAMETIME)
      SDL_Delay(TARGET_FRAMETIME - dt);
    else if (RUNNING != STOP)
      fprintf(stderr, "this is lag.\n");
  }

  inputs_free(input_down);
  world_free(&w);
  background_component_free(back);

  free_asset_store();
  free_grid_functions();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

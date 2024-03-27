#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>

#include "components.h"
#include "data_structures/asset_manager.h"
#include "data_structures/ecs.h"
#include "input.h"
#include "renderer/camera.h"
#include "renderer/sprite.h"
#include "renderer/ui.h"
#include "util.h"

int main() {
  init_asset_manager();

  HANDLE_ERROR(SDL_Init(SDL_INIT_VIDEO) < 0, SDL_GetError(), abort());
  atexit(SDL_Quit);
  HANDLE_ERROR(TTF_Init() < 0, SDL_GetError(), abort());
  atexit(TTF_Quit);
  SDL_Window *window =
      SDL_CreateWindow("test", 100, 100, 640, 360, SDL_WINDOW_SHOWN);

  HANDLE_ERROR(!window, SDL_GetError(), abort());
  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  HANDLE_ERROR(!renderer, SDL_GetError(), {
    SDL_DestroyWindow(window);
    abort();
  });

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
  Camera cam = {.x = 100, .y = 100, .zoom = 1};

  init_world(&w);

  // Entity *test_e = spawn_entity(&w);

  Position *test_pos = malloc(sizeof(Position));
  *test_pos = (Position){.x = 155, .y = 250};
  // ecs_add_component(&w, test_e, COMP_POSITION, test_pos);

  Sprite *test_sprite = malloc(sizeof(Sprite));
  SDL_Rect *size = malloc(sizeof(SDL_Rect));
  SDL_Rect *test_rect = malloc(sizeof(SDL_Rect));
  SDL_Color *test_color = malloc(sizeof(SDL_Color));
  Text *test_text = malloc(sizeof(Text));
  SDL_QueryTexture(test_tex, 0, 0, &size->w, &size->h);
  SDL_QueryTexture(test_tex, 0, 0, &test_rect->w, &test_rect->h);
  size->x = 0;
  size->y = 0;
  test_rect->x = 10;
  test_rect->y = 10;
  // Background *test_background = malloc(sizeof(Background));
  Clickable *test_clickable = malloc(sizeof(Clickable));
  // Minimap *test_minimap = malloc(sizeof(Minimap));
  KeyEvent *test_key_event = malloc(sizeof(KeyEvent));

  *test_color = (SDL_Color){.r = 0, .g = 255, .b = 0, .a = 255};
  *test_sprite = (Sprite){.texture = test_tex, .rect = size};
  *test_text = (Text){.str = "uwu\nuwu\n", .color = test_color};

  // *test_background = (Background){.sprite = test_sprite, .rect = size};
  *test_clickable =
      (Clickable){.sprite = test_sprite, .rect = test_rect, .text = test_text};
  *test_key_event = clickable_event;

  // ecs_add_component(&w, test_e, COMP_SPRITE, test_sprite);
  // ecs_add_component(&w, test_e, COMP_BACKGROUND, test_background);
  // ecs_add_component(&w, test_e, COMP_CLICKABLE, test_clickable);
  spawn_clickable(&w, test_clickable, test_key_event);

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
    render_ui(&w, renderer);

    SDL_RenderPresent(renderer);

    // delay before next frame
    dt = min(TARGET_FRAMETIME, SDL_GetTicks() - start_time);
    if (running && dt != TARGET_FRAMETIME)
      SDL_Delay(TARGET_FRAMETIME - dt);
  }

  inputs_free(input_down);
  world_free(&w);
  free(size);
  free(test_sprite);
  free(test_rect);
  free(test_color);
  free(test_text);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

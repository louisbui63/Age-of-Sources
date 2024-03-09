#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>
#include <stdio.h>

#include "components.h"
#include "ecs.h"
#include "util.h"
#include "input.h"

int main() {
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
  SDL_FreeSurface(test_bmp);

  World w = world_new();

  init_world(&w);

  // dt is the frametime from last frame
  Uint32 dt = TARGET_FRAMETIME;
  SDL_Event event;
  Inputs *inputs = inputs_new();
  int running = 1;
  for (;running;) {
    Uint32 start_time = SDL_GetTicks();

    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_KEYDOWN:
          inputs_update_key_press(inputs,event.key.keysym.sym,true);
          break;
        case SDL_KEYUP:
          inputs_update_key_press(inputs,event.key.keysym.sym,false);
          break;
        case SDL_MOUSEBUTTONDOWN:
          inputs_update_mouse_button_press(inputs,event.button.button,true);
          break;
        case SDL_MOUSEBUTTONUP:
          inputs_update_mouse_button_press(inputs,event.button.button,false);
          break;
        case SDL_QUIT:
          running = 0;
          break;
      }
    }
    
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);

    dt = min(TARGET_FRAMETIME, SDL_GetTicks() - start_time);
    if(running && dt != TARGET_FRAMETIME) SDL_Delay(TARGET_FRAMETIME - dt);
  }
  inputs_free(inputs);
}

#include <string.h>

#include "../components.h"
#include "../data_structures/asset_manager.h"
#include "button.h"

extern int RUNNING;

Clickable *spawn_button(World *w, SDL_Renderer *renderer, SDL_Window *window,
                        void (*event)(World *w, SDL_Renderer *renderer,
                                      SDL_Window *window),
                        char *t, int xp, int yp) {
  Clickable *click = malloc(sizeof(Clickable));
  click->sprite = malloc(sizeof(Sprite));
  click->rect = malloc(sizeof(SDL_Rect));
  click->sprite->rect = malloc(sizeof(SDL_Rect));
  click->text = malloc(sizeof(Text));
  click->text->color = malloc(sizeof(SDL_Color));
  click->is_clicked = 0;
  click->click_event = event;
  *click->text->color = (SDL_Color){.r = 0, .g = 255, .b = 0, .a = 255};
  click->text->str = malloc(strlen(t) + 1);
  strcpy(click->text->str, t);
  click->rect->x = xp;
  click->rect->y = yp;
  click->rect->w = 100;
  click->rect->h = 30;
  click->sprite->rect->x = 0;
  click->sprite->rect->y = 0;
  click->sprite->rect->w = 100;
  click->sprite->rect->h = 30;
  click->sprite->texture =
      get_texture("./asset/sprites/button.bmp", renderer, window);
  KeyEvent *key_event = malloc(sizeof(KeyEvent));
  *key_event = clickable_event;
  Entity *e = spawn_clickable(w, click, key_event);
  return click;
}

void spawn_main_menu(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  spawn_main_quit(w, renderer, window);
  spawn_main_option(w, renderer, window);
}

Clickable *spawn_main_quit(World *w, SDL_Renderer *renderer,
                           SDL_Window *window) {
  return spawn_button(w, renderer, window, event_main_quit, "Exit\n", 256, 256);
}

void event_main_quit(__attribute__((unused)) World *w,
                     __attribute__((unused)) SDL_Renderer *renderer,
                     __attribute__((unused)) SDL_Window *window) {
  RUNNING = 0;
}

Clickable *spawn_main_option(World *w, SDL_Renderer *renderer,
                             SDL_Window *window) {
  return spawn_button(w, renderer, window, event_main_option, "Option\n", 256,
                      192);
}

void event_main_option(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  despawn_from_component(w, COMPF_CLICKABLE);
  // despawn_from_component(w, COMP_HOVERABLE);
  spawn_optionmain_menu(w, renderer, window);
}

void spawn_optionmain_menu(World *w, SDL_Renderer *renderer,
                           SDL_Window *window) {
  spawn_optionmain_back(w, renderer, window);
}

Clickable *spawn_optionmain_back(World *w, SDL_Renderer *renderer,
                                 SDL_Window *window) {
  return spawn_button(w, renderer, window, event_optionmain_back, "Back\n", 256,
                      256);
}

void event_optionmain_back(World *w, SDL_Renderer *renderer,
                           SDL_Window *window) {
  despawn_from_component(w, COMPF_CLICKABLE);
  despawn_from_component(w, COMPF_BACKGROUND);
  spawn_main_menu(w, renderer, window);
}

#pragma once

#include "../data_structures/ecs.h"
#include "ui.h"

Clickable *spawn_button(World *w, SDL_Renderer *renderer, SDL_Window *window,
                        void (*event)(World *w, SDL_Renderer *renderer,
                                      SDL_Window *window),
                        char *t, int xp, int yp);

void spawn_main_menu(World *w, SDL_Renderer *renderer, SDL_Window *window);

Clickable *spawn_main_quit(World *w, SDL_Renderer *renderer,
                           SDL_Window *window);

void event_main_quit(World *w, SDL_Renderer *renderer, SDL_Window *window);

Clickable *spawn_main_option(World *w, SDL_Renderer *renderer,
                             SDL_Window *window);

void event_main_option(World *w, SDL_Renderer *renderer, SDL_Window *window);

void spawn_optionmain_menu(World *w, SDL_Renderer *renderer,
                           SDL_Window *window);

Clickable *spawn_optionmain_back(World *w, SDL_Renderer *renderer,
                                 SDL_Window *window);

void event_optionmain_back(World *w, SDL_Renderer *renderer,
                           SDL_Window *window);

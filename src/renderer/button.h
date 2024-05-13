#pragma once

#include "ui.h"

Clickable *spawn_button(World *w, SDL_Renderer *renderer, SDL_Window *window,
                        void (*event)(), char *t, int xp, int yp);

Clickable *spawn_main_quit(World *w, SDL_Renderer *renderer,
                           SDL_Window *window);
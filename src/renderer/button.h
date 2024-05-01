#pragma once

#include "ui.h"

Clickable *spawn_button(World *w, SDL_Renderer *renderer, SDL_Window *window,
                        void (*event)(void *), char *t, int xp, int yp);

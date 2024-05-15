//! @file butt`on.h
#pragma once

#include "../data_structures/ecs.h"
#include "../util.h"
#include "ui.h"

//! This function is used to add clickable in the world with the `click_event`
//! `event`, has the text `t` and it's upper left corner is at the coordinates
//! `xp` `yp`.
Clickable *spawn_button(World *w, SDL_Renderer *renderer, SDL_Window *window,
                        void (*event)(World *w, SDL_Renderer *renderer,
                                      SDL_Window *window),
                        char *t, int xp, int yp);

//! Creates the main menu.
void spawn_main_menu(World *w, SDL_Renderer *renderer, SDL_Window *window);

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

//! Key_event that manages the escape button.
void key_event_escape(World *w, SDL_Renderer *rdr, Entity *entity, Inputs *in,
                      KeyState keystate);

//! Function that returns the sound level as a string with like `sound/128`, for
//! a volume of 37, it returns `37/128`.
char *str_sound_level(World *w, Entity *e);

//! Displays the victory screen
void spawn_victory(World *w, SDL_Renderer *renderer, SDL_Window *window);

//! Displays the defeat screen
void spawn_defeat(World *w, SDL_Renderer *renderer, SDL_Window *window);

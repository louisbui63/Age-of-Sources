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

//! This function is the same as `spawn_button` but uses `clickable_event_mute`
//! instead of `clickable_event` so that it does not make sound.
Clickable *spawn_button_mute(World *w, SDL_Renderer *renderer,
                             SDL_Window *window,
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

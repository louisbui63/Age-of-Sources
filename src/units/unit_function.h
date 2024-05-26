//! @file unit_function.h
#pragma once

#include "../data_structures/ecs.h"
#include "../data_structures/hash_map.h"
#include "../renderer/ui.h"

#include "../components.h"
#include "../players.h"

//! The type of the callback function called when a button in a unit's action
//! grid is pressed
typedef ClickEvent (*GridFunction)(World *, int, Entity *);

//! A dummy `ClickEvent` that does nothing
void empty_click_event(World *w, SDL_Renderer *renderer, SDL_Window *window);

//! A macro that extends to the declation of a `ClickEvent` that spawns a unit
//! of name `unit_name` for `wat` water and `cla` clay, around the position of
//! `entity`, and with `unit` their name in the `Unit` enum
#define slot_spawn_unit(func, unit_name, wat, cla, entity, unit)               \
  void func(World *w, SDL_Renderer *renderer, SDL_Window *window) {            \
    PlayerManager *pm0 = get_player_0(w);                                      \
    if (pm0->water >= wat && pm0->clay >= cla) {                               \
      pm0->water -= wat;                                                       \
      pm0->clay -= cla;                                                        \
      char *c = malloc(sizeof(char) *                                          \
                       (strlen("src/units/unit_" #unit_name ".c") + 1));       \
      strcpy(c, "src/units/unit_" #unit_name ".c");                            \
      Position *p = entity_get_component(w, entity, COMP_POSITION);            \
      spawn_unit(                                                              \
          w, unit, renderer, window,                                           \
          (Position){p->x + rand() % 200 - 100, p->y + rand() % 200 - 100},    \
          0);                                                                  \
    }                                                                          \
  }

ClickEvent tanuki_grid(World *w, int slot, Entity *e);
ClickEvent well_grid(World *w, int slot, Entity *e);
ClickEvent fish_grid(World *w, int slot, Entity *e);
ClickEvent frog_grid(World *w, int slot, Entity *e);
ClickEvent forum_grid(World *w, int slot, Entity *e);
ClickEvent debug_grid(World *w, int slot, Entity *e);
ClickEvent debug2_grid(World *w, int slot, Entity *e);
ClickEvent fort_grid(World *w, int slot, Entity *e);
ClickEvent beaver_grid(World *w, int slot, Entity *e);
ClickEvent casern_grid(World *w, int slot, Entity *e);

//! Returns the `PlayerManager` of player 0, i.e. the human player
PlayerManager *get_player_0(World *w);

//! A global `HashMap` that stores the actions to do when a unit's grid is
//! clicked
extern HashMap GRID_FUNCTION_MAP;

//! Initializes `GRID_FUNCTION_MAP`
void set_grid_functions();

//! Frees `GRID_FUNCTION_MAP`
void free_grid_functions();

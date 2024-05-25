//! @file unit_function.h
#pragma once
#include <stdlib.h>
#include <string.h>

#include "../data_structures/ecs.h"
#include "../data_structures/hash_map.h"
#include "../renderer/ui.h"

#include "../components.h"
#include "../players.h"
#include "../selection.h"

typedef ClickEvent (*GridFunction)(World *, int, Entity *);

void empty_click_event(World *w, SDL_Renderer *renderer, SDL_Window *window);

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
ClickEvent fort_grid(World *w, int slot, Entity *e);
ClickEvent beaver_grid(World *w, int slot, Entity *e);
ClickEvent casern_grid(World *w, int slot, Entity *e);

PlayerManager *get_player_0(World *w);

extern HashMap GRID_FUNCTION_MAP;

void set_grid_functions();

void free_grid_functions();

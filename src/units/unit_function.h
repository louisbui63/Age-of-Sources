//! @file unit_function.h
#pragma once
#include <stdlib.h>
#include <string.h>

#include "../renderer/ui.h"
#include "../data_structures/ecs.h"
#include "../data_structures/hash_map.h"

typedef void (*GridFunction)(World *, int);


void empty_click_event(World *w, SDL_Renderer *renderer, SDL_Window *window);

ClickEvent tanuki_grid(World *w, int slot, Entity *e);
ClickEvent well_grid(World *w, int slot, Entity *e);
ClickEvent fish_grid(World *w, int slot, Entity *e);

extern HashMap GRID_FUNCTION_MAP;

void set_grid_functions();

void free_grid_functions();
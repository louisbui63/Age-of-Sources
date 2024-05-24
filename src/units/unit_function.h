//! @file unit_function.h
#pragma once
#include <stdlib.h>
#include <string.h>

#include "../data_structures/ecs.h"
#include "../data_structures/hash_map.h"

typedef void (*GridFunction)(World *, int);

void tanuki_grid(World *w, int slot);
void well_grid(World *w, int slot);
void fish_grid(World *w, int slot);
void ubeaver_grid(World *w, int slot);

extern HashMap GRID_FUNCTION_MAP;

void set_grid_functions();
inline void free_grid_functions() {
  hash_map_free(&GRID_FUNCTION_MAP /*, hash_map_entry_free_keys*/);
}

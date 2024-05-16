//! @file unit_function.h
#pragma once
#include <stdlib.h>
#include <string.h>

#include "../data_structures/ecs.h"
#include "../data_structures/hash_map.h"

typedef void (*GridFunction)(World *, int);

void tanuki_grid(World *w, int slot);

extern HashMap GRID_FUNCTION_MAP;

inline void set_grid_functions() {
  GRID_FUNCTION_MAP = hash_map_create(hash_str, not_strcmp);

  char *k = malloc(sizeof(char) * (strlen("Tanuki") + 1));
  strcpy(k, "Tanuki");
  GridFunction *v = malloc(sizeof(GridFunction));
  *v = tanuki_grid;
  hash_map_insert(&GRID_FUNCTION_MAP, k, v);
}

inline void free_grid_functions() {
  hash_map_free(&GRID_FUNCTION_MAP /*, hash_map_entry_free_keys*/);
}

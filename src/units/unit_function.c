#include "unit_function.h"

extern HashMap GRID_FUNCTION_MAP;

void set_grid_functions() {
  GRID_FUNCTION_MAP = hash_map_create(hash_str, not_strcmp);

  char *k = malloc(sizeof(char) * (strlen("Tanuki") + 1));
  strcpy(k, "Tanuki");
  GridFunction *v = malloc(sizeof(GridFunction));
  *v = tanuki_grid;
  hash_map_insert(&GRID_FUNCTION_MAP, k, v);

  k = malloc(sizeof(char) * (strlen("Well") + 1));
  strcpy(k, "Well");
  v = malloc(sizeof(GridFunction));
  *v = well_grid;
  hash_map_insert(&GRID_FUNCTION_MAP, k, v);

  k = malloc(sizeof(char) * (strlen("Fish") + 1));
  strcpy(k, "Fish");
  v = malloc(sizeof(GridFunction));
  *v = fish_grid;
  hash_map_insert(&GRID_FUNCTION_MAP, k, v);
}

void free_grid_functions() {
  hash_map_free(&GRID_FUNCTION_MAP /*, hash_map_entry_free_keys*/);
}


void empty_click_event(__attribute__((unused)) World *w, __attribute__((unused)) SDL_Renderer *renderer, __attribute__((unused)) SDL_Window *window) {}
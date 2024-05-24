#include "unit_function.h"

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
  *v = tanuki_grid;
  hash_map_insert(&GRID_FUNCTION_MAP, k, v);

  k = malloc(sizeof(char) * (strlen("Fish") + 1));
  strcpy(k, "Fish");
  v = malloc(sizeof(GridFunction));
  *v = fish_grid;
  hash_map_insert(&GRID_FUNCTION_MAP, k, v);

  k = malloc(sizeof(char) * (strlen("U-Beaver") + 1));
  strcpy(k, "U-Beaver");
  v = malloc(sizeof(GridFunction));
  *v = ubeaver_grid;
  hash_map_insert(&GRID_FUNCTION_MAP, k, v);
}

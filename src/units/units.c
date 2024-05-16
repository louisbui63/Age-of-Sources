#include <stdlib.h>

#include "../renderer/sprite.h"
#include "units.h"

double units_get_tile_speed(UnitTypes u, TileTypes t) {
  double unit_test_speeds[TILE_NUMBER] = {1., 0.00001};
  double *unit_speeds[UNIT_NUMBER] = {unit_test_speeds};
  return unit_speeds[u][t];
}

void unit_component_free(void *temp) {
  Unit *unit = (Unit *)temp;
  free(unit->descr);
  free(unit->name);
  // the sprite shouldn't be freed as it also has to be its own component
  // sprite_component_free(unit->sprite);
  // printf("`%p\n", unit->sprite->rect);
  free(unit->path_to_sprite);
  free(unit);
}

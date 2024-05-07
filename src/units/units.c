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
  sprite_component_free(unit->sprite);
  free(unit->path_to_sprite);
  free(unit);
}

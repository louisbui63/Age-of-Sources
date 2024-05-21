#include "units.h"
#include "../renderer/sprite.h"
#include "../renderer/ui.h"
#include <stdio.h>
#include <stdlib.h>

double units_get_tile_speed(UnitTypes u, TileTypes t) {
  // double unit_test_speeds[TILE_NUMBER] = {1., 1., 1., 0., 0., 1.};
  double unit_speeds[UNIT_NUMBER][TILE_NUMBER] = {{1., 1., 1., 0., 0., 1.}};
  return unit_speeds[u][t];
}

void unit_component_free(void *temp) {
  Unit *unit = (Unit *)temp;
  free(unit->descr);
  free(unit->name);
  // the sprite shouldn't be freed as it also has to be its own component
  // sprite_component_free(unit->sprite);
  free(unit->path_to_sprite);
  free(unit);
}

VEC(Clickable *) unit_actions(UnitTypes t) {}

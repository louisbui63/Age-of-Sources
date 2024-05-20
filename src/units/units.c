#include "units.h"
#include "../renderer/sprite.h"
#include <stdio.h>
#include <stdlib.h>

double units_get_tile_speed(UnitTypes u, TileTypes t) {
  // double unit_test_speeds[TILE_NUMBER] = {1., 1., 1., 0., 0., 1.};
  double unit_speeds[UNIT_NUMBER][TILE_NUMBER] = {{1., 1., 1., 0., 0., 1.}};
  return unit_speeds[u][t];
}

void unit_component_free(void *temp) {
  Unit *unit = (Unit *)temp;
  // free(unit->descr);
  // free(unit->name);
  // the sprite shouldn't be freed as it also has to be its own component
  // sprite_component_free(unit->sprite);
  // free(unit->path_to_sprite)
  free(unit);
}

void unitt_free(UnitT *unit) {
  free(unit->descr);
  free(unit->name);
  sprite_component_free(unit->sprite);
  free(unit->path_to_sprite);
  free(unit);
}

Unit *spawn_unit(UnitTypes t) {
  UnitT *ut = get_unit(t);
  Unit *u = malloc(sizeof(Unit));
  *u = (Unit){.b_dam = ut->b_dam,
              .b_def = ut->b_def,
              .descr = ut->descr,
              .hp = ut->hp,
              .max_hp = ut->hp,
              .name = ut->name,
              .p_dam = ut->p_dam,
              .p_def = ut->p_def,
              .path_to_sprite = ut->path_to_sprite,
              .rg = ut->rg,
              .s_dam = ut->s_dam,
              .s_def = ut->s_def,
              .sprite = ut->sprite,
              .sp = ut->sp};
  return u;
}

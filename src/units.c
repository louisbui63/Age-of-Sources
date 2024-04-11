#include "units.h"

double units_get_tile_speed(UnitTypes u, TileTypes t) {
  double unit_test_speeds[TILE_NUMBER] = {1.,0.00001};
  double *unit_speeds[UNIT_NUMBER] = {unit_test_speeds};
  return unit_speeds[u][t];
}

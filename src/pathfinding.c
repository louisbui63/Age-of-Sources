#include <math.h>

#include "pathfinding.h"

double pathfind_astar_heuristic(UnitTypes u, TilePosition *src,
                                TilePosition *dest) {
  double m = INFINITY;
  for (int i = 0; i < TILE_NUMBER; i++)
    m = m > units_get_tile_speed(u, i) ? units_get_tile_speed(u, i) : m;
  return m * (abs(src->x - dest->x) + abs(src->y - dest->y));
}

Path pathfind_astar(Map m, UnitTypes u, TilePosition *src, TilePosition *dest) {

}

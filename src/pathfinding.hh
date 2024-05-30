//! @file pathfinding.h
#pragma once

#include "data_structures/map.h"
#include "units/units.h"

//! Stores the position of a tile
typedef struct {
  int x;
  int y;
} TilePosition;

//! A `Path` on the global map is succession of tile positions, first index
//! being the start of the path
typedef VEC(TilePosition *) Path;

void path_free(Path p);

//! Returns a minimal `Path` using the A* algorithm
Path pathfind_astar(Map m, UnitTypes u, TilePosition *src, TilePosition *dest);

//! Returns the distance between `src` and `dest` times the cost
//! for crossing a tile (cost = `TILE_NUMBER`/sum(speeds)) (currently using the
//! euclidean distance)
double pathfind_astar_heuristic(UnitTypes u, TilePosition *src,
                                TilePosition *dest);

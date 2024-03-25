//! @file pathfinding.h
#pragma once

#include "data_structures/map.h"
#include "renderer/camera.h"
#include "units.h"

//! stores the position of a tile
typedef struct {
  int x;
  int y;
} TilePosition;

//! a `Path` on the global map is succession of tile positions
typedef VEC(TilePosition) Path;

//! returns a minimal `Path` using the A* algorithm
Path pathfind_astar(Map m, UnitTypes u, TilePosition *src, TilePosition *dest);

//! returns the taxicab distance between `src` and `dest` times the lowest cost
//! for crossing a tile (cost = 1/speed)
double pathfind_astar_heuristic(UnitTypes u, TilePosition *src,
                                TilePosition *dest);

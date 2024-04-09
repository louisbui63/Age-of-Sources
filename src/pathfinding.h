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

//! a `Path` on the global map is succession of tile positions, first index
//! being the start of the path
typedef VEC(TilePosition *) Path;

//! returns a minimal `Path` using the A* algorithm
Path pathfind_astar(Map m, UnitTypes u, TilePosition *src, TilePosition *dest);

//! returns the distance between `src` and `dest` times the lowest cost
//! for crossing a tile (cost = 1/speed) (currently using the euclidean
//! distance)
double pathfind_astar_heuristic(UnitTypes u, TilePosition *src,
                                TilePosition *dest);

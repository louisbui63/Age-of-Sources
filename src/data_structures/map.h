//! @file map.h

#pragma once

//! an `enum` containing all the tiles for the game
typedef enum {
  TILE_PLAIN,

  TILE_NUMBER
} TileTypes;

//! used to store a map as a matrix of `TileTypes` (each value designates a
//! specific type of tile, ex: water, plain...)
typedef TileTypes **Map;

//! `TILE_SIZE` is the ingame length of a tile's edge (tiles being squares)
#define TILE_SIZE 16

//! returns a new `Map` initialized at 0 with size `w*h`
Map map_create(int w, int h);

//! frees a `Map` created with `map_create`
void map_free(Map m);

//! returns the width of a `Map`
#define map_width(m) ((int*)(m))[-2]

//! returns the height of a `Map`
#define map_height(m) ((int*)(m))[-1]

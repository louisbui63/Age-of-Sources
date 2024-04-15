//! @file map.h
#pragma once

#include <stdint.h>

//! an `enum` containing all the tiles for the game
typedef enum {
  TILE_PLAIN,
  TILE_FOREST,

  TILE_NUMBER
} TileTypes;

//! used to store a map as a matrix of `TileTypes` (each value designates a
//! specific type of tile, ex: water, plain...)
typedef TileTypes **Map;

//! A component that contains a `Map`, for rendering purposes. Having more than
//! one such component is undefined behabior.
typedef struct {
  Map map;
} MapComponent;

//! `TILE_SIZE` is the ingame length of a tile's edge (tiles being squares)
#define TILE_SIZE 16

//! frees a `MapComponent`, for use in the ecs.
void map_component_free(void *a);

//! returns a new `Map` initialized at 0 with size `w*h`
Map map_create(int w, int h);

//! frees a `Map` created with `map_create`
void map_free(Map m);

//! returns the width of a `Map`
#define map_width(m) ((int *)(m))[-2]

//! returns the height of a `Map`
#define map_height(m) ((int *)(m))[-1]

//! Creates a `Map` from the bitmap pointed to by `path`. Said bitmap sould be
//! single channel, with 8 bit per color.
Map load_map_from_bmp(char *path);

//! Returns the file name associated with a certain color. Return value should
//! be freed.
char *get_tile_file_name(int8_t id);

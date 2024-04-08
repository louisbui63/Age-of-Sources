//! @file units.h
#pragma once

#include "data_structures/map.h"

//! an `enum` containing all the units for the game
typedef enum {
  UNIT_TEST, // funny name :)

  UNIT_NUMBER
} UnitTypes;

//! returns the speed at which a unit of type `unit` should go on a tile of type
//! `tile`
// #define units_get_tile_speed(unit, tile) unit_speeds[unit][tile]

double units_get_tile_speed(UnitTypes u, TileTypes t);

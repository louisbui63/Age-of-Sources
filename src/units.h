//! @file units.h
#pragma once

#include "data_structures/map.h"

//! an `enum` containing all the units for the game
typedef enum {
  UNIT_TEST, // funny name :)

  UNIT_NUMBER
} UnitTypes;

double unit_test_speeds[TILE_NUMBER] = {1.};

double *unit_speeds[UNIT_NUMBER] = {unit_test_speeds};

//! returns the speed at which a unit of type `unit` should go on a tile of type
//! `tile`
#define units_get_tile_speed(unit, tile) unit_speeds[unit][tile]

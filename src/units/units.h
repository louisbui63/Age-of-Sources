//! @file units.h

#pragma once

#include <stdint.h>

#include "../data_structures/map.h"
#include "../renderer/sprite.h"

//! The body type describes every unit and building in the game.
//! Name matches the body's name, it must not be more than 255 characters long
//! not including '\0', it must not contain '*'.
//! Every number type data must be smaller or equal than 65535.
//! HP corresponds to the body's Health Points. BDam
//! corresponds to the body's Blunt Damage. PDam corresponds to the body's
//! Piercing Damage. SDam corresponds to the body's Special Defence. BDef
//! corresponds to the body's Blunt Defence. PDef corresponds to the body's
//! Piercing Defence. SDef corresponds to the body's Special defence. Sp
//! corresponds to the body's Speed. Rg corresponds to the body's Range. w and h
//! corresponds respectively to the body's width and heigth. A building is
//! differentiated from a unit because a building's SP is 0.
//! Descr matches the body's description, it must not be more than 65535
//! characters long not including '\0', it must not contain '*'.
typedef struct {
  char *name;
  uint16_t hp;          // Health Point
  uint16_t b_dam;       // Blunt damage
  uint16_t p_dam;       // Piercing damage
  uint16_t s_dam;       // Special damage
  uint16_t b_def;       // Blunt defence
  uint16_t p_def;       // Piecing defence
  uint16_t s_def;       // Special defence
  uint16_t rg;          // Range
  uint16_t sp;          // Speed
  Sprite *sprite;       // Sprite of the unit
  char *path_to_sprite; // Path to the sprite of the unit
  char *descr;          // Description
} Unit;

//! an `enum` containing all the units for the game
typedef enum {
  UNIT_TEST, // funny name :)

  UNIT_NUMBER
} UnitTypes;

//! returns the speed at which a unit of type `unit` should go on a tile of type
//! `tile`
// #define units_get_tile_speed(unit, tile) unit_speeds[unit][tile]

double units_get_tile_speed(UnitTypes u, TileTypes t);

void unit_component_free(void *uni);

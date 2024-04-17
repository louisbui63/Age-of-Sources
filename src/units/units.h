//! @file entities.h

#pragma once

#include <stdint.h>

#include "renderer/sprite.h"

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
  char *Name;
  uint16_t HP;    // Health Point
  uint16_t BDam;  // Blunt damage
  uint16_t PDam;  // Piercing damage
  uint16_t SDam;  // Special damage
  uint16_t BDef;  // Blunt defence
  uint16_t PDef;  // Piecing defence
  uint16_t SDef;  // Special defence
  uint16_t Rg;    // Range
  uint16_t Sp;    // Speed
  Sprite *sprite; // Sprite of the unit
  char *Descr;    // Description
} unit;

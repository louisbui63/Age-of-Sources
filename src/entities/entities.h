//! @file entities.h

#pragma once
#include "../util.h"
//! The body type describes every unit and building in the game.
//! Name matches the body's name, it must not be more than 255 characters long
//! not including '\0', it must not contain '*'.
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
  uint HP;     // Health Point
  uint BDam;   // Blunt damage
  uint PDam;   // Piercing damage
  uint SDam;   // Special damage
  uint BDef;   // Blunt defence
  uint PDef;   // Piecing defence
  uint SDef;   // Special defence
  uint Rg;     // Range
  uint Sp;     // Speed
  int w, h;    // Width and height
  char *Descr; // Description
} body;

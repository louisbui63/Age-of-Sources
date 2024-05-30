//! @file units.h

#pragma once

#include <stdint.h>

#include "../data_structures/map.h"
#include "../renderer/camera.h"
#include "../renderer/sprite.h"

//! an `enum` containing all the units for the game
typedef enum {
  UNIT_TEST, // funny name :)
  WELL,
  FURNACE,
  CASERN,
  TOWER,
  KONBINI,
  HOUSE,
  FORT,
  FORUM,
  UWELL,
  UFURNACE,
  UCASERN,
  UTOWER,
  UKONBINI,
  UHOUSE,
  UFORT,
  UFORUM,
  BEAVER,
  UBEAVER,
  BASE_SOLDIER,
  BASE_FISH,
  MAID,
  FROG,
  SHRIMP,
  HIPPO,
  NARVAL,
  PINGU,
  SAMURAI,
  SECU,
  T34,
  DEBUG,
  DEBUG2,
  JELLYFISH,
  UNIT_NUMBER
} UnitTypes;

//! The Unit type describes every unit and building in the game.
//! Every number type data must be smaller or equal than 65535.
//! A building is differentiated from a soldier/builder because a building's sp
//! is 0.
typedef struct {
  //! name matches the unit's name, it must not be more than 255 characters long
  //! not including '\0', it must not contain '*'.
  char *name;

  //! hp corresponds to the unit's current Health Points.
  double hp;
  uint16_t max_hp; // Maximum Health Point

  //! b_dam corresponds to the unit's Blunt Damage.
  uint16_t b_dam; // Blunt damage

  //! p_dam corresponds to the unit's Piercing Damage.
  uint16_t p_dam; // Piercing damage

  //! s_dam corresponds to the unit's Special Defence
  uint16_t s_dam; // Special damage

  //! b_def corresponds to the unit's Blunt Defence.
  uint16_t b_def; // Blunt defence

  //! p_def corresponds to the unit's Piercing Defence.
  uint16_t p_def; // Piecing defence

  //! s_def corresponds to the unit's Special defence.
  uint16_t s_def; // Special defence

  //! rg corresponds to the unit's Range.
  uint16_t rg; // Range

  //! sp corresponds to the unit's Speed.
  uint16_t sp; // Speed

  //! sprite corresponds to the unit's Sprite
  Sprite *sprite; // Sprite of the unit

  //! Self explanatory
  char *path_to_sprite; // Path to the sprite of the unit

  //! descr matches the unit's description, it must not be more than 65535
  //! characters long not including '\0', it must not contain '*'.
  char *descr; // Description
  UnitTypes t;
} Unit;

//! A type similar to `Unit`, it's use is to be stored in the asset manager and
//! be copied to create a `Unit`.
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
  UnitTypes t;
} UnitT;

//! returns the speed at which a unit of type `unit` should go on a tile of type
//! `tile`
double units_get_tile_speed(UnitTypes u, TileTypes t);

//! Self explanatory.
void unit_component_free(void *uni);

//! A component indicating to which player a unit is belonging
typedef struct {
  char owner;
} Ownership;

//! Self explanatory.
void unitt_free(UnitT *u);

//! Creates an Unit of the UnitTypes `t` at the position `p` owned by `owner`.
Entity *spawn_unit(World *w, UnitTypes t, SDL_Renderer *renderer,
                   SDL_Window *window, Position p, char owner);

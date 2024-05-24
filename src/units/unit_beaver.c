#include "../components.h"
#include "../players.h"
#include "../selection.h"
#include "unit_function.h"

//! This file is a template for the unit files, every data about the units must
//! be put inside multiline commentaries and nothing else will use this kind of
//! commentary.

//! The first text is the name of the unit. It must not be more than 255
//! characters long.
/*Beaver*/

//! The second text is the hp of the unit, it must a be positive decimal number
//! smaller or equal than 65535
/*50*/

//! The third text is the b_dam of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*1*/

//! The fourth text is the p_dam of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*0*/

//! The fifth text is the s_dam of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*0*/

//! The sixth text is the b_def of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*4*/

//! The seventh text is the p_def of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*4*/

//! The eigth text is the s_def of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*4*/

//! The ninth text is the rg of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*2*/

//! The tenth text is the sp of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*10*/

//! The elventh text is the w of the unit's sprite, it must a be positive
//! decimal number smaller or equal than 65535
/*32*/

//! The twelfth text is the h of the unit's sprite, it must a be positive
//! decimal number smaller or equal than 65535
/*32*/

//! The thirteenth text is the unit's sprite path, it must be less than 255
//! characters long.
/*asset/sprites/tanuki_beaver.bmp*/

//! The fourtennth text is the unit's description, it must be less than 1024
//! characters long.
/*A hard worker with very little combat experience*/

Entity *BEAVER_ENTITY;

#define beaver_slot_(slot, cl, wa, name, u)                                    \
  void beaver_slot_##slot(World *w, SDL_Renderer *renderer,                    \
                          SDL_Window *window) {                                \
    Bitflag flag = COMPF_PLAYERMANAGER;                                        \
    VEC(EntityRef) ps = world_query(w, &flag);                                 \
    PlayerManager *pm0 =                                                       \
        entity_get_component(w, get_entity(w, ps[0]), COMP_PLAYERMANAGER);     \
    PlayerManager *pm1 =                                                       \
        entity_get_component(w, get_entity(w, ps[1]), COMP_PLAYERMANAGER);     \
    if (pm0->id == 1) {                                                        \
      PlayerManager *tmp = pm0;                                                \
      pm0 = pm1;                                                               \
      pm1 = tmp;                                                               \
    }                                                                          \
    if (pm0->clay >= cl && pm0->water >= wa) {                                 \
      pm0->clay -= cl;                                                         \
      pm0->water -= wa;                                                        \
      char *un =                                                               \
          malloc(sizeof(char) * (strlen("src/units/unit_" #name ".c") + 1));   \
      strcpy(un, "src/units/unit_" #name ".c");                                \
      set_building_selection(w, un, u);                                        \
    }                                                                          \
  }

beaver_slot_(0, 300, 0, well, WELL);
beaver_slot_(1, 200, 100, furnace, FURNACE);
beaver_slot_(2, 600, 400, tanuki_casern, CASERN);
beaver_slot_(3, 600, 400, tanuki_tower, TOWER); // not final values
beaver_slot_(4, 600, 400, konbini_tanuki, KONBINI); // not final values
beaver_slot_(5, 600, 400, tanuki_house, HOUSE); // not final values
beaver_slot_(6, 600, 400, tanuki_fort, FORT); // not final values

// forum -> well -> furn -> cas -> tower -> kon -> house -> fort

ClickEvent beaver_grid(__attribute__((unused)) World *w, int slot, Entity *e) {
  BEAVER_ENTITY = e;
  switch (slot) {
  case 0:
    return beaver_slot_0;
  case 1:
    return beaver_slot_1;
  case 2:
    return beaver_slot_2;
  case 3:
    return beaver_slot_3;
  case 4:
    return beaver_slot_4;
  case 5:
    return beaver_slot_5;
  case 6:
    return beaver_slot_6;
  }
  return empty_click_event;
}

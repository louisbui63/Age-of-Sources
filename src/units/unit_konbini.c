#include "unit_function.h"

//! This file is a template for the unit files, every data about the units must
//! be put inside multiline commentaries and nothing else will use this kind of
//! commentary.

//! The first text is the name of the unit. It must not be more than 255
//! characters long.
/*Konbini*/

//! The second text is the hp of the unit, it must a be positive decimal number
//! smaller or equal than 65535
/*500*/

//! The third text is the b_dam of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*0*/

//! The fourth text is the p_dam of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*0*/

//! The fifth text is the s_dam of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*0*/

//! The sixth text is the b_def of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*100*/

//! The seventh text is the p_def of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*100*/

//! The eigth text is the s_def of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*150*/

//! The ninth text is the rg of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*0*/

//! The tenth text is the sp of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*0*/

//! The elventh text is the w of the unit's sprite, it must a be positive
//! decimal number smaller or equal than 65535
/*64*/

//! The twelfth text is the h of the unit's sprite, it must a be positive
//! decimal number smaller or equal than 65535
/*64*/

//! The thirteenth text is the unit's sprite path, it must be less than 255
//! characters long.
/*asset/sprites/konbini_tanuki.bmp*/

//! The fourtennth text is the unit's description, it must be less than 1024
//! characters long.
/*

*/

void konbini_slot_0(World *w, __attribute__((unused)) SDL_Renderer *renderer,
                       __attribute__((unused)) SDL_Window *window) {
  Bitflag flag = COMPF_PLAYERMANAGER;
  VEC(EntityRef) ps = world_query(w, &flag);
  PlayerManager *pm0 =
      entity_get_component(w, get_entity(w, ps[0]), COMP_PLAYERMANAGER);
  PlayerManager *pm1 =
      entity_get_component(w, get_entity(w, ps[1]), COMP_PLAYERMANAGER);
  if (pm0->id == 1) {
    PlayerManager *tmp = pm0;
    pm0 = pm1;
    pm1 = tmp;
  }

  if (pm0->clay >= 500 && pm0->water >= 500) {
    pm0->clay -= 500;
    pm0->water -= 500;
    pm0->clay_multiplier = 2;
    pm0->water_multiplier = 2;
  }
}

ClickEvent konbini_grid(__attribute__((unused)) World *w,
                        __attribute__((unused)) int slot,
                        __attribute__((unused)) Entity *e) {
  switch (slot) {
    case 0:
      return konbini_slot_0;
  }
  return empty_click_event;
}

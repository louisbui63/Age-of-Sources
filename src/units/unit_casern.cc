#include "unit_function.h"

//! This file is a template for the unit files, every data about the units must
//! be put inside multiline commentaries and nothing else will use this kind of
//! commentary.

//! The first text is the name of the unit. It must not be more than 255
//! characters long.
/*Barracks*/

//! The second text is the hp of the unit, it must a be positive decimal number
//! smaller or equal than 65535
/*600*/

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
/*asset/sprites/tanuki_casern.bmp*/

//! The fourtennth text is the unit's description, it must be less than 1024
//! characters long.
/*

*/

EntityRef CASERN_ENTITY;

slot_spawn_unit(casern_slot_0, samurai, 50, 50, get_entity(w, CASERN_ENTITY),
                SAMURAI)
slot_spawn_unit(casern_slot_1, secu, 50, 50, get_entity(w, CASERN_ENTITY), SECU)
slot_spawn_unit(casern_slot_2, tanuki, 50, 50, get_entity(w, CASERN_ENTITY),
                BASE_SOLDIER)

ClickEvent casern_grid(__attribute__((unused)) World *w,
                       __attribute__((unused)) int slot,
                       __attribute__((unused)) Entity *e) {
  CASERN_ENTITY = e->id;
  switch (slot) {
  case 0:
    return casern_slot_0;
  case 1:
    return casern_slot_1;
  case 2:
    return casern_slot_2;
  }
  return empty_click_event;
}

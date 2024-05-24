#include "../components.h"
#include "../players.h"
#include "../selection.h"
#include "unit_function.h"

//! This file is a template for the unit files, every data about the units must
//! be put inside multiline commentaries and nothing else will use this kind of
//! commentary.

//! The first text is the name of the unit. It must not be more than 255
//! characters long.
/*Forum*/

//! The second text is the hp of the unit, it must a be positive decimal number
//! smaller or equal than 65535
/*2000*/

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
/*50*/

//! The seventh text is the p_def of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*50*/

//! The eigth text is the s_def of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*50*/

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
/*asset/sprites/forum.bmp*/

//! The fourtennth text is the unit's description, it must be less than 1024
//! characters long.
/*The community center of the Tanuki civilization*/

Entity *FORUM_ENTITY;

void forum_slot_0(World *w, SDL_Renderer *renderer, SDL_Window *window) {
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
  printf("%d\n", pm0->water);
  // water&clay 5/s
  if (pm0->water >= 5 * 10 && pm0->clay >= 5 * 10 * 0) {
    pm0->water -= 5 * 10;
    pm0->clay -= 5 * 10 * 0;
    char *c = malloc(sizeof(char) * (strlen("src/units/unit_tanuki.c") + 1));
    strcpy(c, "src/units/unit_tanuki.c");
    Position *p = entity_get_component(w, FORUM_ENTITY, COMP_POSITION);
    spawn_unit(w, BASE_SOLDIER, renderer, window, *p, 0);
  }
}

void forum_slot_1(World *w, SDL_Renderer *renderer, SDL_Window *window) {
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

  if (pm0->water >= 5 * 10) {
    pm0->water -= 5 * 10;
    char *c = malloc(sizeof(char) * (strlen("src/units/unit_beaver.c") + 1));
    strcpy(c, "src/units/unit_beaver.c");
    Position *p = entity_get_component(w, FORUM_ENTITY, COMP_POSITION);
    spawn_unit(w, BEAVER, renderer, window, *p, 0);
  }
}

ClickEvent forum_grid(__attribute__((unused)) World *w, int slot, Entity *e) {
  FORUM_ENTITY = e;
  switch (slot) {
  case 0:
    return forum_slot_0;
  case 1:
    return forum_slot_1;
  }
  return empty_click_event;
}

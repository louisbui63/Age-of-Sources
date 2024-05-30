#include "../components.h"
#include "../players.h"
#include "../selection.h"
#include "unit_function.h"

//! This file is a template for the unit files, every data about the units must
//! be put inside multiline commentaries and nothing else will use this kind of
//! commentary.

//! The first text is the name of the unit. It must not be more than 255
//! characters long.
/*DEBUG*/

//! The second text is the hp of the unit, it must a be positive decimal number
//! smaller or equal than 65535
/*60000*/

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
/*60000*/

//! The seventh text is the p_def of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*60000*/

//! The eigth text is the s_def of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*60000*/

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
/*asset/sprites/debug64.bmp*/

//! The fourtennth text is the unit's description, it must be less than 1024
//! characters long.
/*DEBUG*/

char TARGET = 0;
EntityRef DEBUG_ENTITY;

void unit_debug_slot_0(__attribute__((unused)) World *w,
                       __attribute__((unused)) SDL_Renderer *renderer,
                       __attribute__((unused)) SDL_Window *window) {
  TARGET = !TARGET;
  fprintf(stderr, "[\x1b[95mDEBUG\x1b[0m] Debug target switched to %d\n",
          TARGET);
}
void unit_debug_slot_1(World *w, __attribute__((unused)) SDL_Renderer *renderer,
                       __attribute__((unused)) SDL_Window *window) {
  // construction
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

  PlayerManager *targ = pm0;
  if (TARGET)
    targ = pm1;

  if (targ->construct_multiplier == 1.0)
    targ->construct_multiplier = 100;
  else
    targ->construct_multiplier = 1.0;

  fprintf(
      stderr,
      "[\x1b[95mDEBUG\x1b[0m] Player %d construction multiplier set to %f\n",
      TARGET, targ->construct_multiplier);
}
void unit_debug_slot_2(World *w, __attribute__((unused)) SDL_Renderer *renderer,
                       __attribute__((unused)) SDL_Window *window) {
  // construction
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

  PlayerManager *targ = pm0;
  if (TARGET)
    targ = pm1;

  if (targ->damage_multiplier == 1.0)
    targ->damage_multiplier = 1e10;
  else
    targ->damage_multiplier = 1.0;

  fprintf(stderr,
          "[\x1b[95mDEBUG\x1b[0m] Player %d damage multiplier set to %f\n",
          TARGET, targ->damage_multiplier);
}
void unit_debug_slot_3(World *w, __attribute__((unused)) SDL_Renderer *renderer,
                       __attribute__((unused)) SDL_Window *window) {
  // construction
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

  PlayerManager *targ = pm0;
  if (TARGET)
    targ = pm1;

  if (targ->clay_multiplier == 1.0)
    targ->clay_multiplier = 100.;
  else
    targ->clay_multiplier = 1.0;

  fprintf(stderr,
          "[\x1b[95mDEBUG\x1b[0m] Player %d clay  multiplier set to %f\n",
          TARGET, targ->clay_multiplier);
}
void unit_debug_slot_4(World *w, __attribute__((unused)) SDL_Renderer *renderer,
                       __attribute__((unused)) SDL_Window *window) {
  // construction
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

  PlayerManager *targ = pm0;
  if (TARGET)
    targ = pm1;

  if (targ->water_multiplier == 1.0)
    targ->water_multiplier = 100.;
  else
    targ->water_multiplier = 1.0;

  fprintf(stderr,
          "[\x1b[95mDEBUG\x1b[0m] Player %d water multiplier set to %f\n",
          TARGET, targ->water_multiplier);
}

void unit_debug_slot_5(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  char *c = malloc(sizeof(char) * (strlen("src/units/unit_maid.c") + 1));
  strcpy(c, "src/units/unit_maid.c");
  Position *p =
      entity_get_component(w, get_entity(w, DEBUG_ENTITY), COMP_POSITION);
  spawn_unit(w, MAID, renderer, window,
             (Position){p->x + rand() % 200 - 100, p->y + rand() % 200 - 100},
             0);
}

void unit_debug_slot_6(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  char *c = malloc(sizeof(char) * (strlen("src/units/unit_hippo.c") + 1));
  strcpy(c, "src/units/unit_hippo.c");
  Position *p =
      entity_get_component(w, get_entity(w, DEBUG_ENTITY), COMP_POSITION);
  spawn_unit(w, HIPPO, renderer, window,
             (Position){p->x + rand() % 200 - 100, p->y + rand() % 200 - 100},
             1);
}
void unit_debug_slot_7(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  char *c = malloc(sizeof(char) * (strlen("src/units/unit_frog.c") + 1));
  strcpy(c, "src/units/unit_frog.c");
  Position *p =
      entity_get_component(w, get_entity(w, DEBUG_ENTITY), COMP_POSITION);
  spawn_unit(w, FROG, renderer, window,
             (Position){p->x + rand() % 200 - 100, p->y + rand() % 200 - 100},
             1);
}
void unit_debug_slot_8(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  char *c = malloc(sizeof(char) * (strlen("src/units/unit_gun_shrimp.c") + 1));
  strcpy(c, "src/units/unit_gun_shrimp.c");
  Position *p =
      entity_get_component(w, get_entity(w, DEBUG_ENTITY), COMP_POSITION);
  spawn_unit(w, SHRIMP, renderer, window,
             (Position){p->x + rand() % 200 - 100, p->y + rand() % 200 - 100},
             1);
}
void unit_debug_slot_9(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  char *c = malloc(sizeof(char) * (strlen("src/units/unit_narval.c") + 1));
  strcpy(c, "src/units/unit_narval.c");
  Position *p =
      entity_get_component(w, get_entity(w, DEBUG_ENTITY), COMP_POSITION);
  spawn_unit(w, NARVAL, renderer, window,
             (Position){p->x + rand() % 200 - 100, p->y + rand() % 200 - 100},
             1);
}
void unit_debug_slot_10(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  char *c = malloc(sizeof(char) * (strlen("src/units/unit_jellyfish.c") + 1));
  strcpy(c, "src/units/unit_jellyfish.c");
  Position *p =
      entity_get_component(w, get_entity(w, DEBUG_ENTITY), COMP_POSITION);
  spawn_unit(w, JELLYFISH, renderer, window,
             (Position){p->x + rand() % 200 - 100, p->y + rand() % 200 - 100},
             1);
}

ClickEvent debug_grid(__attribute__((unused)) World *w,
                      __attribute__((unused)) int slot, Entity *e) {
  DEBUG_ENTITY = e->id;
  switch (slot) {
  case 0:
    return unit_debug_slot_0;
  case 1:
    return unit_debug_slot_1;
  case 2:
    return unit_debug_slot_2;
  case 3:
    return unit_debug_slot_3;
  case 4:
    return unit_debug_slot_4;
  case 5:
    return unit_debug_slot_5;
  case 6:
    return unit_debug_slot_6;
  case 7:
    return unit_debug_slot_7;
  case 8:
    return unit_debug_slot_8;
  case 9:
    return unit_debug_slot_9;
  case 10:
    return unit_debug_slot_10;
  }
  return empty_click_event;
}

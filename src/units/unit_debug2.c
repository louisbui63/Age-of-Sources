#include "../components.h"
#include "../data_structures/asset_manager.h"
#include "../players.h"
#include "../selection.h"
#include "unit_function.h"

//! This file is a template for the unit files, every data about the units must
//! be put inside multiline commentaries and nothing else will use this kind of
//! commentary.

//! The first text is the name of the unit. It must not be more than 255
//! characters long.
/*DEBUG (2)*/

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

EntityRef DEBUG2_ENTITY;

SDL_Renderer *get_renderer_l(World *w) {
  uint64_t mask = COMPF_RENDERER;
  VEC(EntityRef) er = world_query(w, &mask);
  Entity *e = get_entity(w, er[0]);
  Renderer *r = entity_get_component(w, e, COMP_RENDERER);
  return r->r;
}

SDL_Window *get_window_l(World *w) {
  uint64_t mask = COMPF_WINDOW;
  VEC(EntityRef) er = world_query(w, &mask);
  Entity *e = get_entity(w, er[0]);
  Window *wi = entity_get_component(w, e, COMP_WINDOW);
  return wi->w;
}

void unit_debug2_slot_0(World *w,
                        __attribute__((unused)) SDL_Renderer *renderer,
                        __attribute__((unused)) SDL_Window *window) {
  Entity *e = get_entity(w, DEBUG2_ENTITY);
  Sprite *sp = entity_get_component(w, e, COMP_SPRITE);
  sp->texture = get_texture("asset/sprites/tour_poisson.bmp", get_renderer_l(w),
                            get_window_l(w));
}
void unit_debug2_slot_1(World *w,
                        __attribute__((unused)) SDL_Renderer *renderer,
                        __attribute__((unused)) SDL_Window *window) {
  Entity *e = get_entity(w, DEBUG2_ENTITY);
  Sprite *sp = entity_get_component(w, e, COMP_SPRITE);
  sp->texture = get_texture("asset/sprites/unectas_casern.bmp",
                            get_renderer_l(w), get_window_l(w));
}
void unit_debug2_slot_2(World *w,
                        __attribute__((unused)) SDL_Renderer *renderer,
                        __attribute__((unused)) SDL_Window *window) {
  Entity *e = get_entity(w, DEBUG2_ENTITY);
  Sprite *sp = entity_get_component(w, e, COMP_SPRITE);
  sp->texture = get_texture("asset/sprites/unectas_fort.bmp", get_renderer_l(w),
                            get_window_l(w));
}
void unit_debug2_slot_3(World *w,
                        __attribute__((unused)) SDL_Renderer *renderer,
                        __attribute__((unused)) SDL_Window *window) {
  Entity *e = get_entity(w, DEBUG2_ENTITY);
  Sprite *sp = entity_get_component(w, e, COMP_SPRITE);
  sp->texture = get_texture("asset/sprites/poisson_konbini.bmp",
                            get_renderer_l(w), get_window_l(w));
}
void unit_debug2_slot_4(World *w,
                        __attribute__((unused)) SDL_Renderer *renderer,
                        __attribute__((unused)) SDL_Window *window) {
  Entity *e = get_entity(w, DEBUG2_ENTITY);
  Sprite *sp = entity_get_component(w, e, COMP_SPRITE);
  sp->texture = get_texture("asset/sprites/poisson_maison.bmp",
                            get_renderer_l(w), get_window_l(w));
}

ClickEvent debug_grid2(__attribute__((unused)) World *w,
                       __attribute__((unused)) int slot, Entity *e) {
  DEBUG2_ENTITY = e->id;
  switch (slot) {
  case 0:
    return unit_debug2_slot_0;
  case 1:
    return unit_debug2_slot_1;
  case 2:
    return unit_debug2_slot_2;
  case 3:
    return unit_debug2_slot_3;
  case 4:
    return unit_debug2_slot_4;
  }
  return empty_click_event;
}

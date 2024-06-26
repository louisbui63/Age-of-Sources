//! @file selection.h
#pragma once
#include "data_structures/ecs.h"
#include "input.h"
#include "units/units.h"
#include <SDL2/SDL.h>

//! This component is a flag that marks something as selectable. A `Position`
//! and a `Sprite` is still required so that we can now where and how big it is.
typedef struct {
  char is_ghost;
} Selectable;

//! The type of the ongoing selection, i.e wether it is used for unit selection
//! or for placement
typedef enum {
  Normal,
  Building,
} SelectionType;

//! A component that manages selection
typedef struct {
  SelectionType type;
  Vec2 start;
  Vec2 end;
  char is_selecting;
  VEC(EntityRef) selected;
  char *building;
  UnitTypes building_utype;
  int water_cost;
  int clay_cost;
} Selector;

//! A `KeyEvent` that manages selections
void selection_event(World *w, SDL_Renderer *r, Entity *e, Inputs *i,
                     KeyState st);

//! draws the selection box when required
void draw_selection(World *w, SDL_Renderer *rdr, SDL_Window *window);

//! Switches the selector to `Building` type for a specified `building` and its
//! UnitTypes `but`
void set_building_selection(World *w, char *building, UnitTypes but, int water,
                            int clay);

//! Self explanatory
void selector_free(void *s);

//! Self explanatory
void render_unit_grid(World *w, EntityRef e);

//! Advances one step in the unit unselection process
void unselect(Selector *s);

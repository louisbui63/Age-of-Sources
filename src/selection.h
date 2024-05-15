//! @file parser.h
#pragma once
#include "data_structures/ecs.h"
#include "input.h"
#include <SDL2/SDL.h>

//! This component is a flag that marks something as selectable. A `Position`
//! and a `Sprite` is still required so that we can now where and how big it is.
typedef struct {
} Selectable;

typedef struct {
  Vec2 start;
  Vec2 end;
  char is_selecting;
  VEC(EntityRef) selected;
} Selector;

//! A `KeyEvent` that manages selections
void selection_event(World *w, SDL_Renderer *r, Entity *e, Inputs *i,
                     KeyState st);

//! draws the selection box when required
void draw_selection(World *w, SDL_Renderer *rdr, SDL_Window *window);

void selector_free(void *s);

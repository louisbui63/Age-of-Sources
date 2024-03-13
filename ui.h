//! @file ui.h
#pragma once
#include "sprite.h"

//! Entities with this component are the background of the user interface
typedef struct {
  Sprite *sprite;
  SDL_Rect *rect;
} Background;

//! Entities with this component start an action when clicked on
typedef struct {
  Sprite *sprite;
  SDL_Rect *rect;
} Clickable;

//! Component that corresponds to the minimap
typedef struct {
  Sprite *sprite;
  SDL_Rect *rect;
} Minimap;

//! Entities with this component show text when hovered
typedef struct {
  SDL_Rect *rect;
  char *text;
} Hoverable;

void render_ui(World *w, SDL_Renderer *rdr);

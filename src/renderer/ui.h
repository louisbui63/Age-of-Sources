//! @file ui.h
#pragma once
#include "../data_structures/ecs.h"
#include "../input.h"
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
  Uint8 is_clicked;
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

//! Renders any entity that has user interface related component
void render_ui(World *w, SDL_Renderer *rdr);

//! Adds a clickable to the world
Entity *spawn_clickable(World *w, Clickable *object, KeyEvent *event);

//! The `KeyEvent` of the entities associated with a clickable component, it
//! checks if it is clicked on
void clickable_event(World *w, Entity *entity, Inputs *in, KeyState keystate);

//! This function is used to render the entities associated with a hoverable
//! component
void render_hoverable(SDL_Rect *rect, char *text);

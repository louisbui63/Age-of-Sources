//! @file ui.h
#pragma once
#include "../data_structures/ecs.h"
#include "../input.h"
#include "sprite.h"

typedef struct {
  char *str;
  SDL_Color *color;
} Text;

//! Entities with this component are the background of the user interface
typedef struct {
  Sprite *sprite;
  SDL_Rect *rect;
} Background;

//! Entities with this component start an action when clicked on
typedef struct {
  Sprite *sprite;
  SDL_Rect *rect;
  Text *text;
  Uint8 is_clicked;
  void (*click_event)();
} Clickable;

//! Component that corresponds to the minimap
typedef struct {
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

//! The `KeyEvent` of the entities associated with a clickable component,
//! there are different cases, if the mouse is out of the sprite,
//! it set `is_clicked` to 0 as for doing nothing,
//! if the left click is pressed on the sprite,
//! it will be set to 1 and if it is set to 1 and the click is released then it
//! will be set to 2. The idea is that if set to 1
//! there will be a visual change by darkening the sprite
//! and if it set to 2 it will start the action linked to the sprite.
//! It must be noted that if you click on the sprite, mouve your mouse out and
//! then release the click it will do nothing as a way to correct missclicks.
void clickable_event(World *w, SDL_Renderer *rdr, Entity *entity, Inputs *in,
                     KeyState keystate);

//! This function is used to render the entities associated with a hoverable
//! component
void render_hoverable(SDL_Rect *rect, char *text);

void hoverable_component_free(void *tmp);

void minimap_component_free(void *temp);

void background_component_free(void *temp);

void clickable_component_free(void *temp);

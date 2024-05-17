//! @file ui.h
#pragma once
#include "../data_structures/ecs.h"
#include "../input.h"
#include "sprite.h"

//! Type that corresponds to the text that should be rendered on entities such
//! as `Clickable`and `Hoverable`.
typedef struct {
  char *str;
  SDL_Color *color;
  int padding;
} Text;

//! Entities with this component are the background of the user interface
typedef struct {
  Sprite *sprite;
  SDL_Rect *rect;
} Background;

//! Entities with this type start an action when clicked on.
//! The value of is_clicked depends if and how it is clicked on,
//! when is_clicked is equal to one it means the left click is pressed on over
//! the clickable and that either it was already equal to one before or that it
//! was being clicked on.
//! If it is equal to two, it means the value was one and the click was released
//! while over it. It activates the clickable's click_event.
typedef struct {
  Sprite *sprite;
  SDL_Rect *rect;
  Text *text;
  Uint8 is_clicked;
  void (*click_event)(World *w, SDL_Renderer *renderer, SDL_Window *window);
} Clickable;

//! Type that corresponds to the minimap
typedef struct {
  SDL_Rect *rect;
} Minimap;

//! Type with this component show text when hovered
typedef struct {
  SDL_Rect *rect;
  char *text;
} Hoverable;

//! Type used to render text that is not constant sur as hp, sound volume or
//! ressources.
typedef struct {
  char *(*get_text)(World *w, Entity *e);
  SDL_Rect *rect;
} Actualised_Text;

//! Renders any entity that has user interface related component
void render_ui(World *w, SDL_Renderer *rdr, SDL_Window *wi);

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

void text_component_free(void *temp);

void actualised_text_component_free(void *temp);

//! Creates a black background that will be rendered before everything else.
Background *spawn_backbackground(SDL_Renderer *rdr, SDL_Window *window);

void null_click_event(__attribute__((unused)) World *w,
                      __attribute__((unused)) SDL_Renderer *renderer,
                      __attribute__((unused)) SDL_Window *window);

//! Change `inner` so that it becomes the biggest rectangle of same ratio that
//! can fit into `outer` padded by `padding` pixels.
void biggest_possible_rectangle_centered(SDL_Rect *outer, SDL_Rect *inner,
                                         int padding);

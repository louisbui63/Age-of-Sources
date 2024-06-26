//! @file ui.h
#pragma once
#include "../data_structures/ecs.h"
#include "../input.h"
#include "../units/units.h"
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

//! The type of the callback function called when a `Clickable` is clicked
typedef void (*ClickEvent)(World *w, SDL_Renderer *renderer,
                           SDL_Window *window);

//! Entities with this type start an action when clicked on.
//! The value of `is_clicked` depends on if and how it is clicked on,
//! when `is_clicked` is equal to one it means the left click is pressed on over
//! the clickable and that either it was already equal to one before or that it
//! was being clicked on.
//! If it is equal to two, it means the value was one and the click was released
//! while over it. It activates the `Clickable`'s `click_event`.
typedef struct {
  Sprite *sprite;
  SDL_Rect *rect;
  Text *text;
  Uint8 is_clicked;
  ClickEvent click_event;
} Clickable;

//! Type that corresponds to the minimap
typedef struct {
  SDL_Rect *rect;
} Minimap;

//! Type used to render text that is not constant such as hp, sound volume or
//! ressources.
typedef struct {
  char *(*get_text)(World *w, Entity *e);
  SDL_Rect *rect;
  SDL_Color *color;
} ActualisedText;

//! Entities with this component show text when hovered
typedef struct {
  SDL_Rect *rect;
  ActualisedText *text;
  UnitTypes t;
} Hoverable;

//! Renders any entity that has user interface related components
void render_ui(World *w, SDL_Renderer *rdr, SDL_Window *wi);

//! Adds a clickable to the world
Entity *spawn_clickable(World *w, Clickable *object, KeyEvent *event);

//! The `KeyEvent` of the entities associated with a clickable component.
//! There are different cases: if the mouse is out of the sprite,
//! it set `is_clicked` to 0 as for doing nothing,
//! if the left click is pressed on the sprite,
//! it will be set to 1 and if it is set to 1 and the click is released then it
//! will be set to 2. The idea is that if it is set to 1
//! there will be a visual change by darkening the sprite
//! and if it is set to 2 it will start the action linked to the sprite.
//! It must be noted that if you click on the sprite, move your mouse out and
//! then release the click it will do nothing as a way to correct missclicks.
void clickable_event(World *w, SDL_Renderer *rdr, Entity *entity, Inputs *in,
                     KeyState keystate);

//! This functon is the same as `clickable_event` but it does not make sound.
void clickable_event_mute(World *w, SDL_Renderer *rdr, Entity *entity,
                          Inputs *in, KeyState keystate);

//! This function is used to render the entities associated with a hoverable
//! component
void render_hoverable(SDL_Rect *rect, char *text);

//! Self explanatory.
void hoverable_component_free(void *tmp);

//! Self explanatory.
void minimap_component_free(void *temp);

//! Self explanatory.
void background_component_free(void *temp);

//! Self explanatory.
void clickable_component_free(void *temp);

//! Self explanatory.
void text_component_free(void *temp);

//! Self explanatory.
void actualised_text_component_free(void *temp);

//! Creates a black background that will be rendered before everything else.
Background *spawn_backbackground(SDL_Renderer *rdr, SDL_Window *window);

//! A click_event that must be used when a clickable shouldn't do anything. This
//! function ought to be useless outside of debug.
void null_click_event(World *w, SDL_Renderer *renderer, SDL_Window *window);

//! Changes `inner` so that it becomes the biggest rectangle of same ratio that
//! can fit into `outer` padded by `padding` pixels and centers it.
void biggest_possible_rectangle_centered(SDL_Rect *outer, SDL_Rect *inner,
                                         int padding);

//! Changes `inner` so that it becomes the biggest rectangle of same ratio that
//! can fit into `outer` padded by `padding` pixels.
void biggest_possible_rectangle(SDL_Rect *outer, SDL_Rect *inner, int padding);

//! This function adds an `Actualised_Text`to the world that will show the game
//! state in the upper left corner of the game.
ActualisedText *render_game_state(World *w);

//! This function returns the string that represents a specific value
//! of `RUNNING`. The argument are not used but they are there for type
//! consistency. The string is padded with spaces at the end so that when the
//! text is rendered they all begin at the same place
char *running_to_str(World *w, Entity *e);

//! Self explanatory.
SDL_Renderer *get_renderer(World *w);

//! Self explanatory.
SDL_Window *get_window(World *w);

//! This function is used to get the text that will appear when the button to
//! create an unit is hovered.
char *unit_hover_text(World *w, Entity *e);

//! @file input.h
#pragma once
#include "data_structures/ecs.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef Uint8 KeyState;
//! the instant the key is pressed
#define KEY_PRESSED 0
//! the instant the key is released
#define KEY_RELEASED 1
//! starts on press (included), ends on release (not included)
#define KEY_DOWN 2

//! describes any of the following: SDL_BUTTON_LEFT,SDL_BUTTON_MIDDLE,
//! SDL_BUTTON_RIGHT
typedef Uint8 MouseButton;

//! stores keys and mouse buttons
typedef struct {
  //! uses SDL Scancodes as indices
  int *keys;
  //! number of keys currently in
  Uint64 key_nb;
  //! 1st bit = mb_left; 2nd bit = mb_middle; 3rd bit = mb_right
  char mouse;
} Inputs;

//! type of callback functions for the key events
typedef void (*KeyEvent)(World *, Entity *, Inputs *, KeyState);

//! creates a new Inputs instance
Inputs *inputs_new();

//! frees the Inputs instance
void inputs_free(Inputs *);

//! the state of a key accessed using SDL_Scancode
//! !!!!!!!!!! this does not take into account non QWERTY keyboards / remaps
//! !!!!!!!!!! bool inputs_is_key_in_from_scancode(Input*,SDL_Scancode)
#define inputs_is_key_in_from_scancode(inputs, scancode)                       \
  ((inputs)->keys[(scancode)])

//! the state of a key accessed using SDL_KeyCode
//! bool inputs_is_key_in(Inputs*, SDL_KeyCode)
#define inputs_is_key_in(inputs, key)                                          \
  ((inputs)->keys[SDL_GetScancodeFromKey(key)])

//! the state of a mouse button
//! bool inputs_is_mouse_button_in(Inputs*,MouseButton)
#define inputs_is_mouse_button_in(inputs, button)                              \
  (((inputs)->mouse >> ((button)-1)) & 1)

//! updates the state of a key using SDL_Scancode
//! !!!!!!!!!! this does not take into account non QWERTY keyboards / remaps
//! !!!!!!!!!! void inputs_update_key_in_from_scancode(Input*,SDL_Scancode,bool)
void inputs_update_key_in_from_scancode(Inputs *inputs, SDL_Scancode scancode,
                                        bool new_val);

//! updates the state of a key using SDL_KeyCode
//! void inputs_update_key_in(Input*,SDL_KeyCode,bool)
#define inputs_update_key_in(inputs, key, new_val)                             \
  (inputs_update_key_in_from_scancode(inputs, SDL_GetScancodeFromKey(key),     \
                                      new_val))

//! updates the state of a mouse button
//! MouseButton inputs_update_mouse_button_in(Input*,MouseButton,bool)
#define inputs_update_mouse_button_in(inputs, button, new_val)                 \
  ((inputs)->mouse = (((!!(new_val)) << ((button)-1)) |                        \
                      ((inputs)->mouse & (~(1 << ((button)-1))))))

//! calls all the callbacks for the keyevent
void inputs_run_callbacks(World *, Inputs *, KeyState);

//! Checks if the mouse is in the rectangle
Uint8 mouse_in_rect(SDL_Rect *rect);

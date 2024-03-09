#pragma once
#include <stdbool.h>

// stores the state of keys and mouse buttons (i.e. pressed or not)
typedef struct {
  int* keys; // uses SDL Scancodes as indices
  char mouse; // 1st bit = mb_left; 2nd bit = mb_middle; 3rd bit = mb_right
} Inputs;

// creates a new Inputs instance
Inputs* inputs_new();

// frees the Inputs instance
void inputs_free(Inputs*);

// the state of a key accessed using SDL_Scancode (true -> being pressed)
// !!!!!!!!!! this does not take into account non QWERTY keyboards !!!!!!!!!!
// inputs_is_key_pressed_from_scancode(Input*,SDL_Scancode)
#define inputs_is_key_pressed_from_scancode(inputs,scancode) ((inputs)->keys[(scancode)])

// the state of a key accessed using SDL_KeyCode (true -> being pressed)
// inputs_is_key_pressed(Inputs*, SDL_KeyCode)
#define inputs_is_key_pressed(inputs,key) ((inputs)->keys[SDL_GetScancodeFromKey(key)])


// the state of a mouse button (true -> being pressed)
// inputs_is_mouse_button_pressed(Inputs*,SDL_BUTTON_...)
#define inputs_is_mouse_button_pressed(inputs,button) (((inputs)->mouse >> ((button)-1)) & 1)


// updates the state of a key using SDL_Scancode
// !!!!!!!!!! this does not take into account non QWERTY keyboards !!!!!!!!!!
// inputs_update_key_press_from_scancode(Input*,SDL_Scancode,bool)
#define inputs_update_key_press_from_scancode(inputs,scancode,new_val) ((inputs)->keys[(scancode)] = (new_val))

// updates the state of a key using SDL_KeyCode
// inputs_update_key_press(Input*,SDL_KeyCode,bool)
#define inputs_update_key_press(inputs,key,new_val) ((inputs)->keys[SDL_GetScancodeFromKey(key)] = (new_val))


// updates the state of a mouse button
// inputs_update_mouse_button_press(Input*,SDL_BUTTON_...,bool)
#define inputs_update_mouse_button_press(inputs,button,new_val) ((((new_val)!=0)<<(button-1))|((inputs)->mouse&(~(1<<(button-1)))))
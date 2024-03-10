#include "input.h"

#include "../ecs.h"
#include "../input.h"
#include "../util.h"
#include "../vec.h"
#include <stdint.h>

#include <stdio.h>

typedef struct {
  int y;
  double uwu;
} TestComp;

int global_count_mine_dont_touch = 0;

void test_keyevent_1(Entity *e, Inputs *in, KeyState ks) {
  global_count_mine_dont_touch += 1;
}

void test_keyevent_2(Entity *e, Inputs *in, KeyState ks) {
  if (ks == KEY_PRESSED && inputs_is_mouse_button_in(in, SDL_BUTTON_MIDDLE))
    global_count_mine_dont_touch += 1;
}

int test_input() {
  // inputs_new test
  Inputs *in = inputs_new();
  ASSERT(in->mouse == 0);
  ASSERT(in->key_nb == 0);
  for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    ASSERT(in->keys[i] == 0);

  // inputs_is_key_in_from_scancode test
  ASSERT(!inputs_is_key_in_from_scancode(in, SDL_SCANCODE_A));
  ASSERT(!inputs_is_key_in_from_scancode(in, SDL_SCANCODE_F16));
  ASSERT(!inputs_is_key_in_from_scancode(in, SDL_SCANCODE_MINUS));

  // inputs_is_key_in test
  ASSERT(!inputs_is_key_in(in, SDLK_6));
  ASSERT(!inputs_is_key_in(in, SDLK_F16));
  ASSERT(!inputs_is_key_in(in, SDLK_TAB));

  // inputs_is_mouse_button_in test
  ASSERT(!inputs_is_mouse_button_in(in, SDL_BUTTON_LEFT));
  ASSERT(!inputs_is_mouse_button_in(in, SDL_BUTTON_RIGHT));
  ASSERT(!inputs_is_mouse_button_in(in, SDL_BUTTON_MIDDLE));

  // inputs_update_key_in_from_scancode & inputs_is_key_in_from_scancode test
  inputs_update_key_in_from_scancode(in, SDL_SCANCODE_F16, 1);
  ASSERT(inputs_is_key_in_from_scancode(in, SDL_SCANCODE_F16));
  inputs_update_key_in_from_scancode(in, SDL_SCANCODE_F16, 0);
  ASSERT(!inputs_is_key_in_from_scancode(in, SDL_SCANCODE_F16));

  // inputs_update_key_in & inputs_is_key_in test
  inputs_update_key_in(in, SDLK_6, 0);
  ASSERT(!inputs_is_key_in(in, SDLK_6));
  inputs_update_key_in(in, SDLK_6, 1);
  ASSERT(inputs_is_key_in(in, SDLK_6));

  inputs_update_key_in(in, SDLK_TAB, 745);
  ASSERT(inputs_is_key_in(in, SDLK_TAB));
  inputs_update_key_in(in, SDLK_TAB, 0);
  ASSERT(!inputs_is_key_in(in, SDLK_TAB));

  // inputs_update_mouse_button_in & inputs_is_mouse_button_in test
  inputs_update_mouse_button_in(in, SDL_BUTTON_LEFT, 0);
  ASSERT(!inputs_is_mouse_button_in(in, SDL_BUTTON_LEFT));
  inputs_update_mouse_button_in(in, SDL_BUTTON_LEFT, 1);
  ASSERT(inputs_is_mouse_button_in(in, SDL_BUTTON_LEFT));

  inputs_update_mouse_button_in(in, SDL_BUTTON_MIDDLE, 349);
  ASSERT(inputs_is_mouse_button_in(in, SDL_BUTTON_MIDDLE));
  ASSERT(inputs_is_mouse_button_in(in, SDL_BUTTON_LEFT));
  ASSERT(!inputs_is_mouse_button_in(in, SDL_BUTTON_RIGHT));

  // inputs_run_callbacks test
  World w = world_new();
  register_component(&w, TestComp);
  register_component(&w, float);
  register_component(&w, KeyEvent);
  register_component(&w, uint8_t);
  register_component(&w, int16_t);

  Bitflag c = 4;

  register_system_requirement(&w, c);

  for (int i = 0; i < 32; i++) {
    Entity *e = spawn_entity(&w);
    TestComp *t = malloc(sizeof(TestComp));
    TestComp tmp = {i, (double)i / 2};
    *t = tmp;
    ecs_add_component(&w, e, 0, t);
    uint8_t *u = malloc(sizeof(uint8_t));
    *u = i + 1;
    ecs_add_component(&w, e, 3, u);
    if (i < 15) {
      float *v = malloc(sizeof(float));
      *v = (float)i;
      ecs_add_component(&w, e, 1, v);
    }

    if (i % 2 == 0) {
      KeyEvent *ke = malloc(sizeof(KeyEvent));
      if (i < 15)
        *ke = test_keyevent_1;
      else
        *ke = test_keyevent_2;
      ecs_add_component(&w, e, 2, ke);
    }
  }

  global_count_mine_dont_touch = 0;
  inputs_run_callbacks(&w, in, KEY_RELEASED);
  ASSERT(global_count_mine_dont_touch == 8);

  global_count_mine_dont_touch = 0;
  ASSERT(inputs_is_mouse_button_in(in, SDL_BUTTON_MIDDLE));
  inputs_run_callbacks(&w, in, KEY_PRESSED);
  ASSERT(global_count_mine_dont_touch == 16);

  inputs_free(in);

  return 0;
}

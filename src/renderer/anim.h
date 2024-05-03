//! @file anim.h
#pragma once

#include "../util.h"
#include "SDL2/SDL.h"

#define ANIM_STEP 16

typedef enum { Idle, Moving, Attacking, Noop } AnimState;

typedef struct {
  uint frame;
  SDL_Rect current;
  AnimState state;
  //! max indicates the last frame for each animation that is still valid
  int max[3];
} Animator;

void advance_anim_state(Animator *a, AnimState as);

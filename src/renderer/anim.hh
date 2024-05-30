//! @file anim.h
#pragma once

#include "../units/units.h"
#include "../util.h"

//! The number of renderer frames for which each animation frame is displayed
#define ANIM_STEP 20

//! The state of an animation, i.e. the kind of movement that is being animated.
typedef enum { Idle, Moving, Attacking, Noop } AnimState;

//! A component that marks an entity as being subject to animations
typedef struct {
  uint frame;
  SDL_Rect current;
  //! The current state of the animation, `Noop` leads to undefined behavior
  //! (note however that this is an unreachable state unless you modify the
  //! component directly)
  AnimState state;
  //! `max` indicates the last frame for each animation that is still valid
  int max[3];
  //! wether the sprite must be vertically flipped
  char flipped;
} Animator;

//! Runs an animation step on the animator, changing the frame if required, and
//! changing the state if `as` isn't `Noop`. Also changes the flipped status of
//! the Animator if `flipped != -1`
void advance_anim_state(Animator *a, AnimState as, char flipped);

//! Builds a new animator for a specific unit
Animator animator_new(Unit *unit_kind);

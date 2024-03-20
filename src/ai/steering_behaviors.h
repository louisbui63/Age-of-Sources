//! @file steering_behavior.h
#pragma once

#include "../util.h"

typedef struct {
  float rotation;
  Vec2 velocity;
  Vec2 position;
  Vec2 steering;
  float max_speed;
} SteerManager;

typedef enum {
  //! This step is not done
  INTERMEDIATE,
  //! This step is done, and there must be a next one
  ONGOING,
  //! This step is done, and we can stop
  COMPLETE,
} BehaviorStatus;

BehaviorStatus behavior_seek(SteerManager *s, Vec2 target);

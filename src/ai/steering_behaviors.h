//! @file steering_behavior.h
#pragma once

#include "../data_structures/vec.h"
#include "../util.h"

//! The `SteerManager` is a componenent that manages movement for an entity
//! according to steering behaviors as defined in *Steering Behaviors For
//! Autonomous Characters* (see resources.md). `max_force` can be set to
//! `INFINITY` (available in `math.h`) if steering should be instantaneous.
typedef struct {
  float max_speed;
  float max_force;
  float mass;
  float awareness;
  float bounding_circle;

  float rotation;
  Vec2 velocity;
  Vec2 position;

  Vec2 steering;
} SteerManager;

//! The `SteerObstacle` is a component that indicates that an entity should be
//! avoided when computing obstacle avoidance behaviors
typedef struct {
  float bounding_circle;
  Vec2 position;
} SteerObstacle;

//! The status of the last behavioral step
typedef enum {
  //! This step is not done
  INTERMEDIATE,
  //! This step is done, and there must be a next one
  ONGOING,
  //! This step is done, and we can stop
  COMPLETE,
} BehaviorStatus;

BehaviorStatus behavior_seek(SteerManager *s, Vec2 target);
BehaviorStatus behavior_obstacle_avoidance(SteerManager *s,
                                           VEC(SteerObstacle) obstacles);
BehaviorStatus behavior_complete(SteerManager *s);

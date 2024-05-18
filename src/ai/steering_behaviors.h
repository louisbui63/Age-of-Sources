//! @file steering_behavior.h
#pragma once

#include "../data_structures/vec.h"
#include "../pathfinding.h"
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

  Path current_path;
} SteerManager;

//! The `SteerObstacle` is a component that indicates that an entity should be
//! avoided when computing obstacle avoidance behaviors.
typedef struct {
  //! the radius of the circle that's considered as an obstacle. Should be the
  //! incircle of the sprite for units and the arithmetic mean of the incircle
  //! and the circumcircle for buildings.
  float bounding_circle;
  Vec2 position;
} SteerObstacle;

//! The status of the last behavioral step
typedef enum {
  //! This step is not done
  INTERMEDIATE,
  //! This step is done, and there must be a next one
  ONGOING,
  //! This step is done, but the current path must be adapted
  INTERRUPTED,
  //! This step is done, and we can stop
  COMPLETE,
} BehaviorStatus;

//! Instructs `s` to seek `target`
BehaviorStatus behavior_seek(SteerManager *s, Vec2 target);
//! Instructs `s` to avoid the obstacles in `obstacles`
BehaviorStatus behavior_obstacle_avoidance(SteerManager *s,
                                           VEC(SteerObstacle *) obstacles);
//! Completes a behavioral cycle for `s`, *id est* updates its velocity and
//! position
BehaviorStatus behavior_complete(SteerManager *s);

//! Instructs `s` to follow its internal path
BehaviorStatus behavior_follow_path(SteerManager *s);

//! converts between tile position and world coordinates
Vec2 tile2pos(TilePosition *tp);

//! converts between tile position and world coordinates
TilePosition pos2tile(Vec2 *tp);

#include "steering_behaviors.h"
#include <math.h>

BehaviorStatus behavior_seek(SteerManager *s, Vec2 target) {

  Vec2 target_vel =
      v2mul(s->max_speed, v2normalize(v2sub(s->position, target)));
  s->steering = v2sub(target_vel, s->velocity);
  return ONGOING;
}

BehaviorStatus behavior_obstacle_avoidance(SteerManager *s,
                                           VEC(SteerObstacle) obstacles) {
  Vec2 bl = v2mul(s->awareness, v2normalize(s->velocity));
  int best = -1;
  float bdist = INFINITY;
  for (uint i = 0; i < vec_len(obstacles); i++) {
    float cdist = v2dot(bl, v2sub(obstacles[i].position, s->position));
    if (v2len(v2sub(v2add(s->position, v2mul(cdist, bl)),
                    obstacles[i].position)) <
        s->bounding_circle + obstacles[i].bounding_circle) {
      if (cdist < bdist) {
        best = i;
        bdist = cdist;
      }
    }
  }

  if (best != -1) {
  }
}

BehaviorStatus behavior_complete(SteerManager *s) {
  Vec2 steering_force = v2truncate(s->steering, s->max_force);
  Vec2 acceleration = v2div(steering_force, s->mass);
  s->velocity = v2truncate(v2add(s->velocity, acceleration), s->max_speed);
  s->position = v2add(s->position, s->velocity);

  if (v2len(s->velocity))
    s->rotation = v2angle(s->velocity);

  return COMPLETE;
}

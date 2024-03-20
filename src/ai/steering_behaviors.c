#include "steering_behaviors.h"

BehaviorStatus behavior_seek(SteerManager *s, Vec2 target) {

  Vec2 target_vel =
      v2mul(s->max_speed, v2normalize(v2sub(s->position, target)));
  s->steering = v2sub(target_vel, s->velocity);
  return ONGOING;
}

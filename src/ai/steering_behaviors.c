#include "steering_behaviors.h"
#include "../components.h"
#include "../units/units.h"
#include <math.h>

BehaviorStatus behavior_seek(SteerManager *s, Vec2 target) {

  Vec2 target_vel =
      v2mul(s->max_speed, v2normalize(v2sub(target, s->position)));
  s->steering = v2sub(target_vel, s->velocity);
  return ONGOING;
}

BehaviorStatus behavior_obstacle_avoidance(SteerManager *s,
                                           VEC(SteerObstacle *) obstacles) {
  Vec2 bl = v2mul(s->awareness, v2normalize(s->velocity));
  int best = -1;
  float bdist = INFINITY;
  for (uint i = 0; i < vec_len(obstacles); i++) {
    float cdist = v2dot(bl, v2sub(obstacles[i]->position, s->position));
    if (v2len(v2sub(v2add(s->position, v2mul(cdist, bl)),
                    obstacles[i]->position)) <
        s->bounding_circle + obstacles[i]->bounding_circle) {
      if (cdist < bdist && cdist > obstacles[i]->bounding_circle) {
        best = i;
        bdist = cdist;
      }
    }
  }

  if (best != -1) {
    Vec2 avoidance =
        v2mul(5, v2normalize(v2sub(bl, obstacles[best]->position)));
    s->steering = avoidance;
    return INTERRUPTED;
  }
  return ONGOING;
}

BehaviorStatus behavior_complete(World *w, Unit *u, SteerManager *s) {
  Vec2 steering_force = v2truncate(s->steering, s->max_force);
  Vec2 acceleration = v2div(steering_force, s->mass);
  s->velocity = v2truncate(v2add(s->velocity, acceleration), s->max_speed);

  TilePosition tp = pos2tile(&s->position);
  Bitflag bf = COMPF_MAPCOMPONENT;
  VEC(EntityRef) mapv = world_query(w, &bf);
  Entity *emap = get_entity(w, mapv[0]);
  MapComponent *mapc = entity_get_component(w, emap, COMP_MAPCOMPONENT);
  s->position = v2add(
      s->position,
      v2mul(units_get_tile_speed(u->t, mapc->map[tp.x][tp.y]), s->velocity));

  if (v2len(s->velocity))
    s->rotation = v2angle(s->velocity);

  return COMPLETE;
}

Vec2 tile2pos(TilePosition *tp) {
  return (Vec2){tp->x * TILE_SIZE + (TILE_SIZE >> 1),
                tp->y * TILE_SIZE + (TILE_SIZE >> 1)};
}

TilePosition pos2tile(Vec2 *tp) {
  return (TilePosition){(tp->x - (TILE_SIZE >> 1)) / TILE_SIZE,
                        (tp->y - (TILE_SIZE >> 1)) / TILE_SIZE};
}

BehaviorStatus behavior_follow_path(SteerManager *s) {
  while (v2len(v2sub(tile2pos(s->current_path[0]), s->position)) < TILE_SIZE) {
    free(s->current_path[0]);
    vec_remove(s->current_path, 0);
    if (!vec_len(s->current_path))
      break;
  }
  if (vec_len(s->current_path))
    behavior_seek(s, tile2pos(s->current_path[0]));
  return ONGOING;
}

void steer_manager_component_free(void *a) {
  SteerManager *stm = a;
  if (stm->current_path)
    path_free(stm->current_path);

  free(a);
}

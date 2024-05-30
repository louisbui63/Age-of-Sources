#include "movement.h"
#include "../actionnable.h"
#include "../components.h"
#include "../data_structures/bitflag.h"
#include "../renderer/anim.h"
#include "steering_behaviors.h"

void move_units(World *w) {
  VEC(SteerObstacle *) obs = vec_new(SteerObstacle *);

  Bitflag mask = COMPF_STEEROBSTACLE;
  EntityRef *er = world_query(w, &mask);
  _Pragma("omp parallel") {
    _Pragma("omp for") for (uint i = 0; i < vec_len(er); i++) {
      EntityRef ei = er[i];
      Entity *e = get_entity(w, ei);
      // vary filters here
      SteerObstacle *sto = entity_get_component(w, e, COMP_STEEROBSTACLE);
      vec_push(obs, sto);
    }

    _Pragma("omp barrier");
  }

  mask = COMPF_POSITION | COMPF_STEERMANAGER;
  er = world_query(w, &mask);
  // this one cannot easily be paralellized
  // _Pragma("omp parallel") {
  //   _Pragma("omp for") {
  for (uint i = 0; i < vec_len(er); i++) {
    EntityRef ei = er[i];
    Entity *e = get_entity(w, ei);
    // vary filters here
    SteerManager *stm = entity_get_component(w, e, COMP_STEERMANAGER);
    if (stm->current_path && vec_len(stm->current_path)) {
      behavior_follow_path(stm);
      if (behavior_obstacle_avoidance(stm, obs) == INTERRUPTED) {
        // We must adapt the path if we find obstacles on the path
        while (v2len(v2sub(tile2pos(stm->current_path[0]), stm->position)) <
               2 * TILE_SIZE) {
          free(stm->current_path[0]);
          vec_remove(stm->current_path, 0);
          if (!vec_len(stm->current_path))
            break;
        }
      }
      Unit *u = entity_get_component(w, e, COMP_UNIT);
      behavior_complete(w, u, stm);
    } else
      stm->velocity = (Vec2){0, 0};

    // _Pragma("omp barrier");
    Position *p = entity_get_component(w, e, COMP_POSITION);
    p->x = stm->position.x;
    p->y = stm->position.y;
    //   }
    // }
  }
  mask = COMPF_POSITION | COMPF_STEERMANAGER | COMPF_STEEROBSTACLE;
  er = world_query(w, &mask);
  _Pragma("omp parallel") {
    _Pragma("omp for") for (uint i = 0; i < vec_len(er); i++) {
      EntityRef ei = er[i];
      Entity *e = get_entity(w, ei);
      SteerObstacle *sto = entity_get_component(w, e, COMP_STEEROBSTACLE);
      Position *p = entity_get_component(w, e, COMP_POSITION);

      sto->position = (Vec2){p->x, p->y};
    }

    _Pragma("omp barrier")
  }

  vec_free(obs);

  // unless I'm thoroughly mistaken, there should be no component that's
  // animated but doesn't use a `SteerManager`
  mask = COMPF_POSITION | COMPF_STEERMANAGER | COMPF_ANIMATOR;
  er = world_query(w, &mask);
  // _Pragma("omp parallel") {
  //   _Pragma("omp for") {
  for (uint i = 0; i < vec_len(er); i++) {
    EntityRef ei = er[i];
    Entity *e = get_entity(w, ei);
    Animator *an = entity_get_component(w, e, COMP_ANIMATOR);
    SteerManager *stm = entity_get_component(w, e, COMP_STEERMANAGER);
    Actionnable *ac = entity_get_component(w, e, COMP_ACTIONNABLE);
    if (ac && ac->act == Attack && stm->current_path &&
        vec_len(stm->current_path)) { // range checking

      Entity *te = get_entity(w, ac->target);
      if (te) {
        Unit *u1 = entity_get_component(w, e, COMP_UNIT);
        Position *p1 = entity_get_component(w, e, COMP_POSITION);

        Unit *u2 = entity_get_component(w, te, COMP_UNIT);
        Position *p2 = entity_get_component(w, te, COMP_POSITION);
        if (!(!u1 || !u2 || !p1 || !p2)) {

          if (v2len(v2sub((Vec2){p1->x, p1->y}, (Vec2){p2->x, p2->y})) <=
              u1->rg * 0.9) {
            path_free(stm->current_path);
            stm->current_path = 0;
          }
        }
      }
    }
    if (stm->velocity.x == 0 && stm->velocity.y == 0) {
      if (ac && ac->act != Lazy) {
        if (actionnate(w, ac, e))
          advance_anim_state(an, Attacking, -1);
      } else
        advance_anim_state(an, Idle, -1);
    } else
      advance_anim_state(an, Moving, stm->velocity.x < 0);
    // }
    // }
    // _Pragma("omp barrier")
  }
}

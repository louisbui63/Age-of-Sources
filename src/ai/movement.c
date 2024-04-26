#include "movement.h"
#include "../components.h"
#include "../data_structures/bitflag.h"
#include "steering_behaviors.h"

void move_units(World *w) {
  VEC(SteerObstacle *) obs = vec_new(SteerObstacle *);

  Bitflag mask = COMPF_STEEROBSTACLE;
  EntityRef *er = world_query(w, &mask);
  _Pragma("omp parallel") {
    _Pragma("omp for") {
      for (uint i = 0; i < vec_len(er); i++) {
        EntityRef ei = er[i];
        Entity *e = get_entity(w, ei);
        // vary filters here
        SteerObstacle *sto = entity_get_component(w, e, COMP_STEEROBSTACLE);
        vec_push(obs, sto);
      }
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
    if (vec_len(stm->current_path)) {
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
      behavior_complete(stm);
    }

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
    _Pragma("omp for") {
      for (uint i = 0; i < vec_len(er); i++) {
        EntityRef ei = er[i];
        Entity *e = get_entity(w, ei);
        SteerObstacle *sto = entity_get_component(w, e, COMP_STEEROBSTACLE);
        Position *p = entity_get_component(w, e, COMP_POSITION);

        sto->position = (Vec2){p->x, p->y};
      }
    }
    _Pragma("omp barrier")
  }
}

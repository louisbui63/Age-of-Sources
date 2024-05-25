#include "actionnable.h"
#include "ai/steering_behaviors.h"
#include "components.h"
#include "construction.h"
#include "pathfinding.h"
#include "players.h"

void apply_damage(Unit *u1, Unit *u2, float multiplier) {
  u2->hp -= multiplier * (pow((double)u1->b_dam / (u2->b_def + 1), 4. / 3) +
                          pow((double)u1->p_dam / (u2->p_def + 1), 2. / 3) +
                          (double)u1->s_dam / (u2->s_def + 1));
}

char actionnate(World *w, Actionnable *ac, Entity *se) {
  switch (ac->act) {
  case Build: {
    Entity *e = get_entity(w, ac->target);
    if (!e) {
      ac->act = Lazy;
      return 0;
    }
    BuildingGhost *bg = entity_get_component(w, e, COMP_BUILDINGGHOST);
    if (!bg) {
      ac->act = Lazy;
      return 0;
    }
    if (bg->construction_done) {
      ac->act = Lazy;
      return 0;
    }
    Bitflag flag = COMPF_PLAYERMANAGER;
    VEC(EntityRef) ps = world_query(w, &flag);
    PlayerManager *pm0 =
        entity_get_component(w, get_entity(w, ps[0]), COMP_PLAYERMANAGER);
    PlayerManager *pm1 =
        entity_get_component(w, get_entity(w, ps[1]), COMP_PLAYERMANAGER);
    if (pm0->id == 1) {
      PlayerManager *tmp = pm0;
      pm0 = pm1;
      pm1 = tmp;
    }
    Ownership *o = entity_get_component(w, se, COMP_OWNERSHIP);
    bg->progress +=
        o->owner ? pm1->construct_multiplier : pm0->construct_multiplier;
    if (bg->progress > bg->max) {
      finish_construction(w, e);
      ac->act = Lazy;
    }
    return 1;
  }
  case Attack: {
    Entity *e = get_entity(w, ac->target);
    if (!e) {
      ac->act = Lazy;
      return 0;
    }
    Unit *u1 = entity_get_component(w, se, COMP_UNIT);
    Position *p1 = entity_get_component(w, se, COMP_POSITION);

    Unit *u2 = entity_get_component(w, e, COMP_UNIT);
    Position *p2 = entity_get_component(w, e, COMP_POSITION);
    if (!u1 || !u2 || !p1 || !p2) {
      ac->act = Lazy;
      return 0;
    }

    if (v2len(v2sub((Vec2){p1->x, p1->y}, (Vec2){p2->x, p2->y})) <= u1->rg) {

      Bitflag flag = COMPF_PLAYERMANAGER;
      VEC(EntityRef) ps = world_query(w, &flag);
      PlayerManager *pm0 =
          entity_get_component(w, get_entity(w, ps[0]), COMP_PLAYERMANAGER);
      PlayerManager *pm1 =
          entity_get_component(w, get_entity(w, ps[1]), COMP_PLAYERMANAGER);
      if (pm0->id == 1) {
        PlayerManager *tmp = pm0;
        pm0 = pm1;
        pm1 = tmp;
      }
      Ownership *o = entity_get_component(w, se, COMP_OWNERSHIP);
      apply_damage(
          u1, u2, (o->owner) ? pm1->damage_multiplier : pm0->damage_multiplier);
      if (u2->hp <= 0) {
        // this fixes any problem when multiple units are killing a single one
        Bitflag flag = COMPF_ACTIONNABLE;
        VEC(EntityRef) u = world_query(w, &flag);
        for (uint i = 0; i < vec_len(u); i++) {
          Actionnable *act =
              entity_get_component(w, get_entity(w, u[i]), COMP_ACTIONNABLE);
          if (act->target == ac->target)
            act->act = Lazy;
        }

        despawn_entity(w, e);
      }
      return 1;
    } else {
      Bitflag bf = COMPF_MAPCOMPONENT;
      VEC(EntityRef) mapv = world_query(w, &bf);
      Entity *emap = get_entity(w, mapv[0]);
      MapComponent *mapc = entity_get_component(w, emap, COMP_MAPCOMPONENT);

      SteerManager *stm = entity_get_component(w, se, COMP_STEERMANAGER);

      Vec2 p_vec2 = (Vec2){.x = p1->x, .y = p1->y};
      TilePosition tpstart = pos2tile(&p_vec2);

      Vec2 mp_vec2 = (Vec2){.x = p2->x, .y = p2->y};

      TilePosition tpend = pos2tile(&mp_vec2);

      if (stm->current_path)
        path_free(stm->current_path);
      stm->current_path = 0;

      Path p = pathfind_astar(mapc->map, UNIT_TEST, &tpstart, &tpend);
      if (p) {
        if (vec_len(p) > 1) {
          free(p[0]);
          vec_remove(p, 0);
        }
        if (vec_len(p) > 1) {
          free(p[0]);
          vec_remove(p, 0);
        }
        stm->current_path = p;
        stm->max_speed = 5.;
        stm->max_force = INFINITY;
      }
    }

  } break;
  case Produce:
  case Lazy:
    break;
  }
  return 0;
}

#include "actionnable.h"
#include "components.h"
#include "construction.h"

void apply_damage(Unit *u1, Unit *u2) {
  u2->hp -= (pow((double)u1->b_dam / (u2->b_def + 1), 4. / 3) +
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
    bg->progress += 1;
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

    if (v2len(v2sub((Vec2){p1->x, p1->y}, (Vec2){p2->x, p2->y})) > u1->rg) {
      apply_damage(u1, u2);
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
    }

  } break;
  case Produce:
  case Lazy:
    break;
  }
  return 0;
}

#include "players.h"
#include "components.h"
#include "units/units.h"

void update_ressources(World *w) {
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

  // some passive ressources to avoid softlocking
  pm1->dwater = 5;
  pm0->dwater = 5;
  pm1->dclay = 5;
  pm0->dclay = 5;

  flag = COMPF_WATERSOURCE;
  VEC(EntityRef) es = world_query(w, &flag);
  for (uint i = 0; i < vec_len(es); i++) {
    Ownership *o =
        entity_get_component(w, get_entity(w, es[i]), COMP_OWNERSHIP);
    if (o->owner == 1)
      pm1->dwater += 10;
    else
      pm0->dwater += 10;
  }
  flag = COMPF_CLAYSOURCE;
  es = world_query(w, &flag);
  for (uint i = 0; i < vec_len(es); i++) {
    Ownership *o =
        entity_get_component(w, get_entity(w, es[i]), COMP_OWNERSHIP);
    if (o->owner == 1)
      pm1->dclay += 10;
    else
      pm0->dclay += 10;
  }

  pm1->water += pm1->dwater * pm1->water_multiplier;
  pm1->clay += pm1->dclay * pm1->clay_multiplier;
  pm0->water += pm0->dwater * pm0->water_multiplier;
  pm0->clay += pm0->dclay * pm0->clay_multiplier;
}

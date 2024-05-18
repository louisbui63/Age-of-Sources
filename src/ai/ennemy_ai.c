#include "ennemy_ai.h"
#include "../components.h"
#include "../players.h"

void ReconsiderAiState(World *w, AiState *ais) {
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

  switch (*ais) {
  case Eco:
    if (is_ai_attacked())
      *ais = Defense;
    if (pm1->dclay + pm1->dwater > pm0->dclay + pm0->dwater)
      *ais = Offense;
    break;
  case Offense:
    if (is_ai_attacked())
      *ais = Defense;
    if (pm1->dclay + pm1->dwater < 1.2 * (pm0->dclay + pm0->dwater))
      *ais = Offense;
    break;
  case Defense:
    if (!is_ai_attacked())
      *ais = Eco;
    break;
  }
}

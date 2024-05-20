#include "actionnable.h"
#include "components.h"
#include "construction.h"

void actionnate(World *w, Actionnable *ac) {
  switch (ac->act) {
  case Build: {
    Entity *e = get_entity(w, ac->target);
    if (!e)
      return;
    BuildingGhost *bg = entity_get_component(w, e, COMP_BUILDINGGHOST);
    if (!bg)
      return;
    if (bg->construction_done) {
      ac->act = Lazy;
      return;
    }
    bg->progress += 1;
    if (bg->progress > bg->max) {
      finish_construction(w, e);
      ac->act = Lazy;
    }
  } break;
  case Attack:
  case Produce:
  case Lazy:
    break;
  }
}

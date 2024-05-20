#include "construction.h"
#include "components.h"

void building_ghost_component_free(void *a) {
  BuildingGhost *bg = a;
  if (!bg->construction_done)
    unit_component_free(bg->u);
  free(a);
}

void finish_construction(World *w, Entity *e) {
  BuildingGhost *bg = entity_get_component(w, e, COMP_BUILDINGGHOST);
  bg->construction_done = 1;

  ecs_add_component(w, e, COMP_UNIT, bg->u);

  despawn_entity(w, e);
}

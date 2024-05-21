#include "construction.h"
#include "components.h"

void building_ghost_component_free(void *a) {
  // BuildingGhost *bg = a;
  // if (!bg->construction_done)
  // unitt_free(bg->u);
  free(a);
}

void finish_construction(World *w, Entity *e) {
  BuildingGhost *bg = entity_get_component(w, e, COMP_BUILDINGGHOST);
  Sprite *sp = entity_get_component(w, e, COMP_SPRITE);
  bg->construction_done = 1;
  Unit *u = malloc(sizeof(Unit));
  *u = (Unit){.b_dam = bg->u->b_dam,
              .b_def = bg->u->b_def,
              .descr = bg->u->descr,
              .hp = bg->u->hp,
              .max_hp = bg->u->hp,
              .name = bg->u->name,
              .p_dam = bg->u->p_dam,
              .p_def = bg->u->p_def,
              .path_to_sprite = bg->u->path_to_sprite,
              .rg = bg->u->rg,
              .s_dam = bg->u->s_dam,
              .s_def = bg->u->s_def,
              .sp = bg->u->sp,
              .sprite = sp,
              .t = bg->unit_type};

  ecs_add_component(w, e, COMP_UNIT, u);

  // despawn_entity(w, e);
}

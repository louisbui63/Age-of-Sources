#include "ui.h"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdint.h>

#include "components.h"
#include "ecs.h"
#include "vec.h"

void render_ui(World *w, SDL_Renderer *rdr) {
  uint64_t mask = COMPF_BACKGROUND;
  EntityRef *er = world_query(w, &mask);
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(w, er[i]);
    Background *b = entity_get_component(e, COMP_BACKGROUND);
    SDL_RenderCopy(rdr, b->sprite->texture, b->sprite->rect, b->rect);
  }
  // Implementer le support des clicks
  mask = COMPF_CLICKABLE;
  EntityRef *er = world_query(w, &mask);
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(w, er[i]);
    Clickable *c = entity_get_component(e, COMP_CLICKABLE);
    SDL_RenderCopy(rdr, c->sprite->texture, c->sprite->rect, c->rect);
  }
  mask = COMPF_MINIMAP;
  EntityRef *er = world_query(w, &mask);
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(w, er[i]);
    Minimap *m = entity_get_component(e, COMP_MINIMAP);
    SDL_RenderCopy(rdr, m->sprite->texture, m->sprite->rect, m->rect);
  }
  // To finish with SDL_TTF
  mask = COMPF_HOVERABLE;
  EntityRef *er = world_query(w, &mask);
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(w, er[i]);
    Hoverable *h = entity_get_component(e, COMP_HOVERABLE);
  }
}

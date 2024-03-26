#include "ui.h"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdint.h>

#include "../components.h"
#include "../data_structures/vec.h"

void render_ui(World *w, SDL_Renderer *rdr) {
  uint64_t mask = COMPF_BACKGROUND;
  EntityRef *er = world_query(w, &mask);
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(w, er[i]);
    Background *b = entity_get_component(w, e, COMP_BACKGROUND);
    SDL_RenderCopy(rdr, b->sprite->texture, b->sprite->rect, b->rect);
  }
  mask = COMPF_CLICKABLE;
  er = world_query(w, &mask);
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(w, er[i]);
    Clickable *c = entity_get_component(w, e, COMP_CLICKABLE);
    if (c->is_clicked) {
      SDL_SetTextureColorMod(c->sprite->texture, (Uint8)100, (Uint8)100,
                             (Uint8)100);
    }
    SDL_RenderCopy(rdr, c->sprite->texture, c->sprite->rect, c->rect);
    if (c->is_clicked) {
      SDL_SetTextureColorMod(c->sprite->texture, (Uint8)255, (Uint8)255,
                             (Uint8)255);
    }
    if (!(c->is_clicked - 2)){
      //script linked to clickable action 
      c->is_clicked = 0;
    }
  }
  mask = COMPF_MINIMAP;
  er = world_query(w, &mask);
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(w, er[i]);
    Minimap *m = entity_get_component(w, e, COMP_MINIMAP);
    SDL_RenderCopy(rdr, m->sprite->texture, m->sprite->rect, m->rect);
  }
  // To finish with SDL_TTF
  mask = COMPF_HOVERABLE;
  // printf("1\n ");
  er = world_query(w, &mask);
  printf("1\n ");
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(w, er[i]);
    Hoverable *h = entity_get_component(w, e, COMP_HOVERABLE);
      //  if (mouse_in_rect(h->rect))
      //    render_hoverable(h->rect, h->text);
  }
}

Entity *spawn_clickable(World *w, Clickable *object, KeyEvent *event) {
  Entity *entity = spawn_entity(w);
  ecs_add_component(w, entity, COMP_CLICKABLE, object);
  ecs_add_component(w, entity, COMP_KEY_EVENT, event);
  return entity;
}

void clickable_event(World *w, Entity *entity, Inputs *in, KeyState keystate) {
  // if (!inputs_is_mouse_button_in(in, SDL_BUTTON_LEFT))
  //   return;
  Clickable *c = entity_get_component(w, entity, COMP_CLICKABLE);
  
  if (mouse_in_rect(c->sprite->rect)) {
    if (keystate == KEY_PRESSED) {
      c->is_clicked = 1;
    } else if ((keystate == KEY_RELEASED) * (c->is_clicked == 1)) {
      c->is_clicked = 2;
    }
  } else
    c->is_clicked = 0;
}

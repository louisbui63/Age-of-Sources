#include "ui.h"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>

#include "../components.h"
#include "../data_structures/vec.h"
#include "../data_structures/asset_manager.h"

void render_ui(World *w, SDL_Renderer *rdr) {
  uint64_t mask = COMPF_BACKGROUND;
  VEC(EntityRef) er = world_query(w, &mask);
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
    // SDL_Texture *tex = c->sprite->texture;
    if (c->is_clicked) {
      SDL_SetTextureColorMod(c->sprite->texture, (Uint8)100, (Uint8)100,
                             (Uint8)100);
    }
    SDL_RenderCopy(rdr, c->sprite->texture, c->sprite->rect, c->rect);
    if (c->text->str[0]){
      TTF_Font *font = get_font("asset/fonts/FiraCodeNerdFont-Retina.ttf", 100);
      SDL_Surface *surf = TTF_RenderText_Solid(font, c->text->str, *c->text->color);
      SDL_Texture *text_texture =  SDL_CreateTextureFromSurface(rdr, surf);
      SDL_RenderCopy(rdr, text_texture, NULL, c->rect);
      SDL_FreeSurface(surf);
      SDL_DestroyTexture(text_texture);
    }  
    if (c->is_clicked) {
      SDL_SetTextureColorMod(c->sprite->texture, (Uint8)255, (Uint8)255,
                             (Uint8)255);
    }
    if (!(c->is_clicked - 2)) {
      // script linked to clickable action
      c->is_clicked = 0;
    }
  }
  mask = COMPF_MINIMAP;
  er = world_query(w, &mask);
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(w, er[i]);
    Minimap *m = entity_get_component(w, e, COMP_MINIMAP);
    SDL_RenderCopy(rdr, m->sprite->texture, NULL, m->rect);
  }
  // To finish with SDL_TTF
  mask = COMPF_HOVERABLE;
  er = world_query(w, &mask);
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(w, er[i]);
    Hoverable *h = entity_get_component(w, e, COMP_HOVERABLE);
    if (mouse_in_rect(h->rect))
      render_hoverable(h->rect, h->text);
  }
}

Entity *spawn_clickable(World *w, Clickable *object, KeyEvent *event) {
  Entity *entity = spawn_entity(w);
  ecs_add_component(w, entity, COMP_CLICKABLE, object);
  ecs_add_component(w, entity, COMP_KEY_EVENT, event);
  return entity;
}

void clickable_event(World *w, Entity *entity, Inputs *in, KeyState keystate) {
  Clickable *c = entity_get_component(w, entity, COMP_CLICKABLE);
  if (!(mouse_in_rect(c->sprite->rect))) {
    c->is_clicked = 0;
    return;
  } else if (!inputs_is_mouse_button_in(in, SDL_BUTTON_LEFT))
    return;
  else if (keystate == KEY_PRESSED) {
    c->is_clicked = 1;
  } else if ((keystate == KEY_RELEASED) * (c->is_clicked == 1))
    c->is_clicked = 2;
}
void render_hoverable(SDL_Rect *rect, char *text) {
  rect = rect + 0;
  text = text + 0;
}

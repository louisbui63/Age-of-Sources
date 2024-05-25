#include "ui.h"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>

#include "../audio/audio.h"
#include "../components.h"
#include "../data_structures/asset_manager.h"
#include "../data_structures/vec.h"
#include "sprite.h"

extern Running RUNNING;

void render_ui(World *w, SDL_Renderer *rdr, SDL_Window *wi) {

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
    if (c->is_clicked) {
      SDL_SetTextureColorMod(c->sprite->texture, (Uint8)100, (Uint8)100,
                             (Uint8)100);
    }
    SDL_RenderCopy(rdr, c->sprite->texture, c->sprite->rect, c->rect);
    if (c->text->str[0]) {
      TTF_Font *font = get_font("asset/fonts/FiraCodeNerdFont-Retina.ttf", 99);
      SDL_Rect r = *(c->rect);
      TTF_SizeUTF8(font, c->text->str, &(r.w), &(r.h));
      SDL_Surface *surf = TTF_RenderText_Blended_Wrapped(font, c->text->str,
                                                         *c->text->color, 0);
      SDL_Texture *text_texture = SDL_CreateTextureFromSurface(rdr, surf);
      // You need to comment or uncomment the three next lines to activate the
      // text shading
      if (c->is_clicked) {
        SDL_SetTextureColorMod(text_texture, (Uint8)100, (Uint8)100,
                               (Uint8)100);
      }

      SDL_Rect t_rect;
      TTF_SizeUTF8(font, c->text->str, &(t_rect.w), &(t_rect.h));
      biggest_possible_rectangle_centered(c->rect, &t_rect, c->text->padding);

      SDL_RenderCopy(rdr, text_texture, NULL, &t_rect);
      SDL_FreeSurface(surf);
      SDL_DestroyTexture(text_texture);
    }
    if (c->is_clicked) {
      SDL_SetTextureColorMod(c->sprite->texture, (Uint8)255, (Uint8)255,
                             (Uint8)255);
    }
    if (!(c->is_clicked - 2)) {
      c->is_clicked = 0;
      c->click_event(w, rdr, wi);
    }
  }

  mask = COMPF_MINIMAP;
  er = world_query(w, &mask);
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(w, er[i]);
    Minimap *m = entity_get_component(w, e, COMP_MINIMAP);
    m = m + 0;
    // SDL_RenderCopy(rdr, m->sprite->texture, NULL, m->rect);
  }

  // To finish with SDL_TTF
  mask = COMPF_HOVERABLE;
  er = world_query(w, &mask);
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(w, er[i]);
    Hoverable *h = entity_get_component(w, e, COMP_HOVERABLE);
    if (mouse_in_rect(rdr, h->rect))
      render_hoverable(h->rect, h->text);
  }

  mask = COMPF_ACTUALISEDTEXT;
  er = world_query(w, &mask);
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(w, er[i]);
    ActualisedText *t = entity_get_component(w, e, COMP_ACTUALISEDTEXT);
    TTF_Font *font = get_font("asset/fonts/FiraCodeNerdFont-Retina.ttf", 99);
    SDL_Rect r = *(t->rect);
    char *text = (t->get_text)(w, e);
    TTF_SizeUTF8(font, text, &(r.w), &(r.h));
    SDL_Surface *surf =
        TTF_RenderUTF8_Blended_Wrapped(font, text, *(t->color), 0);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(rdr, surf);
    SDL_Rect t_rect;
    TTF_SizeUTF8(font, text, &(t_rect.w), &(t_rect.h));
    biggest_possible_rectangle_centered(t->rect, &t_rect, 0);
    free(text);

    SDL_RenderCopy(rdr, text_texture, NULL, &t_rect);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(text_texture);
  }
}

Entity *spawn_clickable(World *w, Clickable *object, KeyEvent *event) {
  Entity *entity = spawn_entity(w);
  ecs_add_component(w, entity, COMP_CLICKABLE, object);
  ecs_add_component(w, entity, COMP_KEY_EVENT, event);
  return entity;
}

void clickable_event(World *w, SDL_Renderer *rdr, Entity *entity, Inputs *in,
                     KeyState keystate) {
  Clickable *c = entity_get_component(w, entity, COMP_CLICKABLE);
  if (!(mouse_in_rect(rdr, c->rect))) {
    c->is_clicked = 0;
    return;
  } else if (!inputs_is_mouse_button_in(in, SDL_BUTTON_LEFT))
    return;
  else if (keystate == KEY_PRESSED) {
    c->is_clicked = 1;
  } else if ((keystate == KEY_RELEASED) * (c->is_clicked == 1)) {
    c->is_clicked = 2;
    play_audio("./asset/sfx/click.wav", 0);
  }
}

void clickable_event_mute(World *w, SDL_Renderer *rdr, Entity *entity,
                          Inputs *in, KeyState keystate) {
  Clickable *c = entity_get_component(w, entity, COMP_CLICKABLE);
  if (!(mouse_in_rect(rdr, c->rect))) {
    c->is_clicked = 0;
    return;
  } else if (!inputs_is_mouse_button_in(in, SDL_BUTTON_LEFT))
    return;
  else if (keystate == KEY_PRESSED) {
    c->is_clicked = 1;
  } else if ((keystate == KEY_RELEASED) * (c->is_clicked == 1)) {
    c->is_clicked = 2;
  }
}

void render_hoverable(SDL_Rect *rect, char *text) {
  rect = rect + 0;
  text = text + 0;
}

void hoverable_component_free(void *tmp) {
  Hoverable *hov = (Hoverable *)tmp;
  text_component_free(hov->text);
  free(hov->rect);
  free(hov);
}

void minimap_component_free(void *temp) {
  Minimap *minm = (Minimap *)temp;
  free(minm->rect);
  free(minm);
}

void clickable_component_free(void *temp) {
  Clickable *click = (Clickable *)temp;
  sprite_component_free(click->sprite);
  free(click->rect);
  text_component_free(click->text);
  free(click);
}

void background_component_free(void *temp) {
  Background *bg = (Background *)temp;
  sprite_component_free(bg->sprite);
  free(bg->rect);
  free(bg);
}

void text_component_free(void *temp) {
  Text *text = (Text *)temp;
  free(text->str);
  free(text->color);
  free(text);
}

void actualised_text_component_free(void *temp) {
  ActualisedText *text = (ActualisedText *)temp;
  free(text->rect);
  free(text->color);
  free(text);
}

Background *spawn_backbackground(SDL_Renderer *rdr, SDL_Window *window) {
  Background *back = malloc(sizeof(Background));
  back->sprite = malloc(sizeof(Sprite));
  back->rect = NULL;
  back->sprite->rect = NULL;
  back->sprite->texture =
      get_texture("./asset/backbackground.bmp", rdr, window);
  return back;
}

void null_click_event(__attribute__((unused)) World *w,
                      __attribute__((unused)) SDL_Renderer *renderer,
                      __attribute__((unused)) SDL_Window *window) {}

void biggest_possible_rectangle_centered(SDL_Rect *outer, SDL_Rect *inner,
                                         int padding) {
  int ow = outer->w - 2 * padding;
  int oh = outer->h - 2 * padding;
  int iw = inner->w;
  int ih = inner->h;
  if ((1.0 * ow) / oh > (1.0 * iw) / ih) {
    inner->h = oh;
    inner->w = (oh * iw) / ih;
  } else if ((1.0 * ow) / oh > (1.0 * iw) / ih) {
    *inner = *outer;
  } else {
    inner->w = ow;
    inner->h = (ow * ih) / iw;
  }
  iw = inner->w;
  ih = inner->h;
  inner->x = (ow + 2 * (outer->x + padding) - iw) / 2;
  inner->y = (oh + 2 * (outer->y + padding) - ih) / 2;
}

ActualisedText *render_game_state(World *w) {
  Entity *e = spawn_entity(w);
  ActualisedText *t = malloc(sizeof(ActualisedText));
  t->color = malloc(sizeof(SDL_Color));
  *(t->color) = (SDL_Color){.r = 255, .g = 255, .b = 255, .a = 255};
  t->rect = malloc(sizeof(SDL_Rect));
  *(t->rect) = (SDL_Rect){.x = 0, .y = 0, .w = 150, .h = 20};
  t->get_text = running_to_str;
  ecs_add_component(w, e, COMP_ACTUALISEDTEXT, t);
  return t;
}

char *running_to_str(__attribute__((unused)) World *w,
                     __attribute__((unused)) Entity *e) {
  char *t = malloc(strlen("Stop          ") + 1);
  switch (RUNNING) {
  case STOP:
    strcpy(t, "Stop          ");
    break;

  case MAIN:
    strcpy(t, "Main          ");
    break;

  case OPTIONMAIN:
    strcpy(t, "OptionMain    ");
    break;

  case IN_GAME:
    strcpy(t, "In_Game       ");
    break;

  case IN_GAMEMENU:
    strcpy(t, "In_GameMenu   ");
    break;

  case IN_GAMEOPTION:
    strcpy(t, "In_GameOption ");
    break;

  case VICTORY:
    strcpy(t, "Victory       ");
    break;

  case DEFEAT:
    strcpy(t, "Defeat        ");
    break;
  }
  return t;
}

SDL_Renderer *get_renderer(World *w) {
  uint64_t mask = COMPF_RENDERER;
  VEC(EntityRef) er = world_query(w, &mask);
  Entity *e = get_entity(w, er[0]);
  Renderer *r = entity_get_component(w, e, COMP_RENDERER);
  return r->r;
}

SDL_Window *get_window(World *w) {
  uint64_t mask = COMPF_WINDOW;
  VEC(EntityRef) er = world_query(w, &mask);
  Entity *e = get_entity(w, er[0]);
  Window *wi = entity_get_component(w, e, COMP_WINDOW);
  return wi->w;
}

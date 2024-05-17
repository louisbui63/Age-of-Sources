#include "selection.h"
#include "components.h"
#include "data_structures/asset_manager.h"
#include "data_structures/ecs.h"
#include "input.h"
#include "renderer/camera.h"
#include "renderer/sprite.h"
#include "ai/steering_behaviors.h"
#include <SDL2/SDL_rect.h>

extern Running RUNNING;

void reset_selection_type(Selector *s) {
  if (s->type == Building)
    free(s->building);
  s->type = Normal;
  SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
}

void set_building_selection(World *w, char *building) {
  Bitflag flag = COMPF_SELECTOR;
  VEC(EntityRef) es = world_query(w, &flag);
  // I'm not responsible if you somehow end up with two selectors and so it
  // doesn't work properly
  Entity *e = get_entity(w, es[0]);
  Selector *sl = entity_get_component(w, e, COMP_SELECTOR);

  sl->is_selecting = 0;
  sl->type = Building;
  sl->building = building;
  SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR));
}

void selection_event(World *w, SDL_Renderer *r, Entity *e, Inputs *i,
                     KeyState st) {
  Selector *s = entity_get_component(w, e, COMP_SELECTOR);

  if (s->type == Normal && RUNNING == IN_GAME) {
    if (inputs_is_mouse_button_in(i, SDL_BUTTON_LEFT)) {
      if (st == KEY_PRESSED) {
        if(get_mouse_position(r).y < 270){
          s->is_selecting = 1;
          vec_clear(s->selected);
          SDL_Point p = get_mouse_position(r);
          s->start = (Vec2){p.x, p.y};
        } else {
          SDL_Point p = get_mouse_position(r);
          s->start = (Vec2){p.x, p.y};
        }
      } else if (st == KEY_DOWN && s->start.y < 270) {
        s->is_selecting = 1;
        SDL_Point p = get_mouse_position(r);
        s->end = (Vec2){p.x, min(p.y,270)};
      } else if (st == KEY_RELEASED && s->start.y < 270) {
        SDL_Point p = get_mouse_position(r);
        s->end = (Vec2){p.x, min(p.y,270)};
        s->is_selecting = 0;

        Bitflag flag = COMPF_CAMERA;
        Camera *cam = entity_get_component(
            w, get_entity(w, world_query(w, &flag)[0]), COMP_CAMERA);

        Position si =
            screen2worldspace(&(Position){s->start.x, s->start.y}, cam);
        Position se = screen2worldspace(&(Position){s->end.x, s->end.y}, cam);
        SDL_Rect sel_rect = {min(si.x, se.x), min(si.y, se.y),
                             max(se.x, si.x) - min(si.x, se.x),
                             max(se.y, si.y) - min(se.y, si.y)};
        flag = COMPF_SELECTABLE | COMPF_POSITION | COMPF_SPRITE;
        VEC(EntityRef) es = world_query(w, &flag);
        for (uint i = 0; i < vec_len(es); i++) {
          Entity *e = get_entity(w, es[i]);
          Sprite *sp = entity_get_component(w, e, COMP_SPRITE);
          Position *p = entity_get_component(w, e, COMP_POSITION);
          if (SDL_PointInRect(&(SDL_Point){p->x, p->y}, &sel_rect) &&
              SDL_PointInRect(
                  &(SDL_Point){p->x + sp->rect->w, p->y + sp->rect->h},
                  &sel_rect)) {
            vec_push(s->selected, es[i]);
          }
        }
      }
    }
    if(inputs_is_mouse_button_in(i,SDL_BUTTON_RIGHT)){
      if(st == KEY_RELEASED){
        Bitflag bf = COMPF_MAPCOMPONENT;
        VEC(EntityRef) mapv = world_query(w,&bf);
        Entity* emap = get_entity(w,mapv[0]);
        MapComponent* mapc = entity_get_component(w,emap,COMP_MAPCOMPONENT);
        for(uint j=0;j<vec_len(s->selected);j++){
          Entity* e = get_entity(w,s->selected[j]);
          SteerManager* stm = entity_get_component(w,e,COMP_STEERMANAGER);
          Position* p = entity_get_component(w,e,COMP_POSITION);
          if(stm != 0){
            Vec2 p_vec2 = (Vec2){.x=p->x,.y=p->y};
            TilePosition tpstart = pos2tile(&p_vec2);

            SDL_Point mp = get_mouse_position(r);
            Vec2 mp_vec2 = (Vec2){.x=mp.x,.y=mp.y};
            TilePosition tpend = pos2tile(&mp_vec2);

            if(stm->current_path) path_free(stm->current_path);
            stm->current_path = pathfind_astar(mapc->map,UNIT_TEST,&tpstart,&tpend);
            stm->max_speed = 15.;
            stm->max_force = INFINITY;
          }
        }
      }
    }
  }
}

//! draws the selection box when required
void draw_selection(World *w, SDL_Renderer *rdr, SDL_Window *window) {
  Bitflag flag = COMPF_SELECTOR;
  VEC(EntityRef) es = world_query(w, &flag);
  // I'm not responsible if you somehow end up with two selectors and so it
  // doesn't work properly
  Entity *e = get_entity(w, es[0]);
  Selector *sl = entity_get_component(w, e, COMP_SELECTOR);
  if (sl->is_selecting) {
    SDL_Texture *t =
        get_texture("asset/sprites/selection_edge.bmp", rdr, window);
    SDL_RenderCopy(
        rdr, t, 0,
        &(SDL_Rect){min(sl->start.x, sl->end.x), sl->start.y,
                    max(sl->start.x, sl->end.x) - min(sl->start.x, sl->end.x),
                    1});
    SDL_RenderCopy(
        rdr, t, 0,
        &(SDL_Rect){min(sl->start.x, sl->end.x), sl->end.y,
                    max(sl->start.x, sl->end.x) - min(sl->start.x, sl->end.x),
                    1});
    SDL_RenderCopy(
        rdr, t, 0,
        &(SDL_Rect){sl->start.x, min(sl->start.y, sl->end.y), 1,
                    max(sl->start.y, sl->end.y) - min(sl->start.y, sl->end.y)});
    SDL_RenderCopy(
        rdr, t, 0,
        &(SDL_Rect){sl->end.x, min(sl->start.y, sl->end.y), 1,
                    max(sl->start.y, sl->end.y) - min(sl->start.y, sl->end.y)});
  }
}

void selector_free(void *s) {
  vec_free(((Selector *)s)->selected);
  free(s);
}

#include "selection.h"
#include "actionnable.h"
#include "ai/steering_behaviors.h"
#include "components.h"
#include "construction.h"
#include "data_structures/asset_manager.h"
#include "data_structures/ecs.h"
#include "input.h"
#include "renderer/camera.h"
#include "renderer/sprite.h"
#include "renderer/ui.h"
#include "units/unit_function.h"
#include "units/units.h"
#include <SDL2/SDL_rect.h>

extern Running RUNNING;

void reset_selection_type(World *w, Selector *s) {
  if (s->type == Building) {
    free(s->building);

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

    pm0->water += s->water_cost;
    pm0->clay += s->clay_cost;
  }
  s->type = Normal;
  SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
}

void set_building_selection(World *w, char *building, UnitTypes but, int water,
                            int clay) {
  Bitflag flag = COMPF_SELECTOR;
  VEC(EntityRef) es = world_query(w, &flag);
  // I'm not responsible if you somehow end up with two selectors and so it
  // doesn't work properly
  Entity *e = get_entity(w, es[0]);
  Selector *sl = entity_get_component(w, e, COMP_SELECTOR);

  if (sl->type == Building) {
    free(sl->building);

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

    pm0->water += sl->water_cost;
    pm0->clay += sl->clay_cost;
  }

  sl->is_selecting = 0;
  sl->type = Building;
  sl->building = building;
  sl->building_utype = but;
  sl->water_cost = water;
  sl->clay_cost = clay;
  SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR));
}

void selection_event(World *w, SDL_Renderer *r, Entity *e, Inputs *i,
                     KeyState st) {
  Selector *s = entity_get_component(w, e, COMP_SELECTOR);

  if (inputs_is_key_in(i, SDLK_m)) {
    char *un = malloc(sizeof(char) * (strlen("src/units/unit_debug.c") + 1));
    strcpy(un, "src/units/unit_debug.c");
    set_building_selection(w, un, DEBUG, 0, 0);
  }

  if (inputs_is_key_in(i, SDLK_TAB) && st == KEY_PRESSED &&
      vec_len(s->selected) > 1) {
    uint i = 0;
    for (; i < vec_len(s->selected) &&
           !entity_get_component(w, get_entity(w, s->selected[i]),
                                 COMP_STEERMANAGER);
         i++)
      ;
    if (i >= vec_len(s->selected))
      i = 0;
    EntityRef er = s->selected[i];
    vec_clear(s->selected);
    vec_push(s->selected, er);
  }

  if (s->type == Normal && RUNNING == IN_GAME) {
    if (inputs_is_mouse_button_in(i, SDL_BUTTON_LEFT)) {
      if (st == KEY_PRESSED) {
        if (get_mouse_position(r).y < 270) {
          s->is_selecting = 1;
          vec_clear(s->selected);
          SDL_Point p = get_mouse_position(r);
          s->start = (Vec2){p.x, p.y};
          despawn_from_component(w, COMPF_CLICKABLE);
        } else {
          SDL_Point p = get_mouse_position(r);
          s->start = (Vec2){p.x, p.y};
        }
      } else if (st == KEY_DOWN && s->start.y < 270) {
        s->is_selecting = 1;
        SDL_Point p = get_mouse_position(r);
        s->end = (Vec2){p.x, min(p.y, 270)};
      } else if (st == KEY_RELEASED && s->start.y < 270) {
        SDL_Point p = get_mouse_position(r);
        s->end = (Vec2){p.x, min(p.y, 270)};
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
        flag =
            COMPF_SELECTABLE | COMPF_POSITION | COMPF_SPRITE | COMPF_OWNERSHIP;
        VEC(EntityRef) es = world_query(w, &flag);
        for (uint i = 0; i < vec_len(es); i++) {
          Entity *e = get_entity(w, es[i]);
          Ownership *o = entity_get_component(w, e, COMP_OWNERSHIP);
          if (o->owner == 1)
            continue;
          BuildingGhost *bg = entity_get_component(w, e, COMP_BUILDINGGHOST);
          if (bg && !bg->construction_done)
            continue;
          Sprite *sp = entity_get_component(w, e, COMP_SPRITE);
          Position *p = entity_get_component(w, e, COMP_POSITION);
          if (SDL_PointInRect(
                  &(SDL_Point){p->x - (int)(sp->rect->w / (2 * cam->zoom)),
                               p->y - (int)(sp->rect->h / (2 * cam->zoom))},
                  &sel_rect) &&
              SDL_PointInRect(
                  &(SDL_Point){p->x + (int)(sp->rect->w / (2 * cam->zoom)),
                               p->y + (int)(sp->rect->h / (2 * cam->zoom))},
                  &sel_rect)) {
            vec_push(s->selected, es[i]);
          }
        }
        if (vec_len(s->selected) == 1)
          render_unit_grid(w, s->selected[0]);
      }
    }
  } else if (s->type == Building && RUNNING == IN_GAME) {
    if (inputs_is_key_in(i, SDLK_ESCAPE) && st == KEY_PRESSED)
      reset_selection_type(w, s);
    else if (inputs_is_mouse_button_in(i, SDL_BUTTON_LEFT) &&
             get_mouse_position(r).y < 270 && st == KEY_RELEASED) {

      SDL_Point pt = get_mouse_position(r);
      Position pworld = (Position){pt.x, pt.y};
      Bitflag bf_cam = COMPF_CAMERA;
      VEC(EntityRef) camv = world_query(w, &bf_cam);
      Entity *ecam = get_entity(w, camv[0]);
      Camera *cam = entity_get_component(w, ecam, COMP_CAMERA);
      pworld = screen2worldspace(&pworld, cam);
      Vec2 vworld = (Vec2){pworld.x, pworld.y};
      TilePosition tp_mouse = pos2tile(&vworld);

      Bitflag bf = COMPF_MAPCOMPONENT;
      VEC(EntityRef) mapv = world_query(w, &bf);
      Entity *emap = get_entity(w, mapv[0]);
      MapComponent *mapc = entity_get_component(w, emap, COMP_MAPCOMPONENT);

      Bitflag flag = COMPF_WINDOW;
      SDL_Window *window = entity_get_component(
          w, get_entity(w, world_query(w, &flag)[0]), COMP_WINDOW);
      if (units_get_tile_speed(s->building_utype,
                               mapc->map[tp_mouse.x][tp_mouse.y])) {
        Entity *e = spawn_entity(w);
        // ownership can only be 0 if building is placed by the player
        Ownership *o = calloc(1, sizeof(Ownership));
        ecs_add_component(w, e, COMP_OWNERSHIP, o);
        // UnitT *u = parse(s->building, r, window);
        UnitT *u = get_unit(s->building_utype, r, window);
        BuildingGhost *bg = malloc(sizeof(BuildingGhost));
        *bg = (BuildingGhost){u, 0, u->hp, 0, s->building_utype};
        ecs_add_component(w, e, COMP_BUILDINGGHOST, bg);

        Sprite *sp = malloc(sizeof(Sprite));
        sp->rect = malloc(sizeof(SDL_Rect));
        *(sp->rect) = *(u->sprite->rect);
        sp->texture = u->sprite->texture;

        ecs_add_component(w, e, COMP_SPRITE, sp);
        Position *p = calloc(1, sizeof(Position));
        *p = pworld;
        ecs_add_component(w, e, COMP_POSITION, p);
        Selectable *sbis = calloc(1, sizeof(Selectable));
        sbis->is_ghost = 1;
        ecs_add_component(w, e, COMP_SELECTABLE, sbis);

        if (s->building_utype == DEBUG)
          finish_construction(w, e);

        reset_selection_type(w, s);
      }
    }
  }
  if (inputs_is_mouse_button_in(i, SDL_BUTTON_RIGHT) && RUNNING == IN_GAME) {
    if (st == KEY_RELEASED) {
      Bitflag bf = COMPF_MAPCOMPONENT;
      VEC(EntityRef) mapv = world_query(w, &bf);
      Entity *emap = get_entity(w, mapv[0]);
      MapComponent *mapc = entity_get_component(w, emap, COMP_MAPCOMPONENT);

      bf = COMPF_CAMERA;
      VEC(EntityRef) camv = world_query(w, &bf);
      Entity *ecam = get_entity(w, camv[0]);
      Camera *cam = entity_get_component(w, ecam, COMP_CAMERA);

      SDL_Point mp = get_mouse_position(r);
      Position mpp = (Position){.x = mp.x, .y = mp.y};
      Position mps = screen2worldspace(&mpp, cam);

      EntityRef action_btarget = UINT64_MAX;
      EntityRef action_atarget = UINT64_MAX;

      Bitflag flag = COMPF_SELECTABLE | COMPF_POSITION | COMPF_SPRITE |
                     COMPF_OWNERSHIP | COMPF_BUILDINGGHOST;
      VEC(EntityRef) es = world_query(w, &flag);
      for (uint i = 0; i < vec_len(es); i++) {
        Entity *e = get_entity(w, es[i]);
        Ownership *o = entity_get_component(w, e, COMP_OWNERSHIP);
        if (o->owner == 1)
          continue;
        Sprite *sp = entity_get_component(w, e, COMP_SPRITE);
        Position *p = entity_get_component(w, e, COMP_POSITION);
        if (SDL_PointInRect(
                &(SDL_Point){mps.x, mps.y},
                &(SDL_Rect){
                    sp->rect->x + p->x - (int)(sp->rect->w / (2 * cam->zoom)),
                    sp->rect->y + p->y - (int)(sp->rect->h / (2 * cam->zoom)),
                    sp->rect->w, sp->rect->h}) &&
            !((BuildingGhost *)entity_get_component(w, e, COMP_BUILDINGGHOST))
                 ->construction_done) {
          action_btarget = es[i];
          break;
        }
      }
      if (action_btarget == UINT64_MAX) {
        Bitflag flag = COMPF_SELECTABLE | COMPF_POSITION | COMPF_SPRITE |
                       COMPF_OWNERSHIP | COMPF_UNIT;
        VEC(EntityRef) es = world_query(w, &flag);
        for (uint i = 0; i < vec_len(es); i++) {
          Entity *e = get_entity(w, es[i]);
          Ownership *o = entity_get_component(w, e, COMP_OWNERSHIP);
          if (o->owner == 0)
            continue;
          Sprite *sp = entity_get_component(w, e, COMP_SPRITE);
          Position *p = entity_get_component(w, e, COMP_POSITION);
          if (SDL_PointInRect(
                  &(SDL_Point){mps.x, mps.y},
                  &(SDL_Rect){
                      sp->rect->x + p->x - (int)(sp->rect->w / (2 * cam->zoom)),
                      sp->rect->y + p->y - (int)(sp->rect->h / (2 * cam->zoom)),
                      sp->rect->w, sp->rect->h})) {
            action_atarget = es[i];
            break;
          }
        }
      }

      for (uint j = 0; j < vec_len(s->selected); j++) {
        Entity *e = get_entity(w, s->selected[j]);
        SteerManager *stm = entity_get_component(w, e, COMP_STEERMANAGER);
        Position *p = entity_get_component(w, e, COMP_POSITION);
        if (stm != 0) {

          Actionnable *ac = entity_get_component(w, e, COMP_ACTIONNABLE);
          if (ac && action_btarget != UINT64_MAX) {
            ac->act = Build;
            ac->target = action_btarget;
          } else if (ac && action_atarget != UINT64_MAX) {
            ac->act = Attack;
            ac->target = action_atarget;
          } else if (ac) {
            ac->act = Lazy;
            ac->target = UINT64_MAX;
          }
          /* constant-expression */
          // Position ps = screen2worldspace(p, cam);
          Vec2 p_vec2 = (Vec2){.x = p->x, .y = p->y};
          TilePosition tpstart = pos2tile(&p_vec2);

          Vec2 mp_vec2 = (Vec2){.x = mps.x, .y = mps.y};

          TilePosition tpend = pos2tile(&mp_vec2);

          if (stm->current_path)
            path_free(stm->current_path);
          stm->current_path = 0;

          Unit *u = entity_get_component(w, e, COMP_UNIT);
          Path p = pathfind_astar(mapc->map, u->t, &tpstart, &tpend);
          if (p) {
            if (vec_len(p) > 1) {
              free(p[0]);
              vec_remove(p, 0);
            }
            if (vec_len(p) > 1) {
              free(p[0]);
              vec_remove(p, 0);
            }
            stm->current_path = p;
            stm->max_speed = 5.;
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
    SDL_RenderCopy(rdr, t, 0,
                   &(SDL_Rect){min(sl->start.x, sl->end.x), sl->end.y,
                               max(sl->start.x, sl->end.x) -
                                   min(sl->start.x, sl->end.x) + 1,
                               1});
    SDL_RenderCopy(rdr, t, 0,
                   &(SDL_Rect){sl->start.x, min(sl->start.y, sl->end.y), 1,
                               max(sl->start.y, sl->end.y) -
                                   min(sl->start.y, sl->end.y) + 1});
    SDL_RenderCopy(rdr, t, 0,
                   &(SDL_Rect){sl->end.x, min(sl->start.y, sl->end.y), 1,
                               max(sl->start.y, sl->end.y) -
                                   min(sl->start.y, sl->end.y) + 1});
  }
}

void selector_free(void *s) {
  Selector *se = s;

  if (se->type == Building)
    free(se->building);
  vec_free(se->selected);
  free(s);
}

void actualise_grid_coordinates(int *x, int *y, int i) {
  int n = 6; // This is the length of a line in the ui + 1
  *x = 135 + (i % n) * 32;
  *y = 270 + 15 + (i / n) * 32;
}

void render_unit_grid(World *w, EntityRef e) {
  UnitTypes t =
      ((Unit *)entity_get_component(w, get_entity(w, e), COMP_UNIT))->t;
  despawn_from_component(w, COMPF_CLICKABLE);
  int i = 0;
  int x = 135;
  int y = 270 + 15;
  Clickable *c;
  SDL_Window *wi = get_window(w);
  SDL_Renderer *r = get_renderer(w);
  KeyEvent *key_event;
  Hoverable *h;
  Entity *en;
  SDL_Rect hrect = (SDL_Rect){.x = 347, .y = 285, .h = 64, .w = 278};
  switch (t) {
  case BEAVER:

    //   // Forum
    //   {actualise_grid_coordinates(&x, &y, i);
    //   c = malloc(sizeof(Clickable));
    //   c->rect = malloc(sizeof(SDL_Rect));
    //   *(c->rect) = (SDL_Rect){.x = x, .y = 0, .h = 32, .w = 32};
    //   c->is_clicked = 0;
    //   c->click_event = beaver_grid(w, i, e);
    //   c->text = malloc(sizeof(Text));
    //   c->text->str = malloc(1);
    //   *(c->text->str) = '\0';
    //   c->text->color = malloc(1);
    //   c->sprite = malloc(sizeof(Sprite));
    //   c->sprite->rect = malloc(sizeof(SDL_Rect));
    //   *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 64, .w = 64};
    //   c->sprite->texture = get_texture("asset/sprites/forum.bmp", r, wi);
    //   key_event = malloc(sizeof(KeyEvent));
    //   *key_event = clickable_event;
    // en = spawn_clickable(w, c, key_event);
    //   i++;}

    // Well
    {
      actualise_grid_coordinates(&x, &y, i);
      c = malloc(sizeof(Clickable));
      c->rect = malloc(sizeof(SDL_Rect));
      *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
      c->is_clicked = 0;
      c->click_event = beaver_grid(w, i, get_entity(w, e));
      c->text = malloc(sizeof(Text));
      c->text->str = malloc(1);
      *(c->text->str) = '\0';
      c->text->color = malloc(1);
      c->sprite = malloc(sizeof(Sprite));
      c->sprite->rect = malloc(sizeof(SDL_Rect));
      *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
      c->sprite->texture = get_texture("asset/sprites/well.bmp", r, wi);
      key_event = malloc(sizeof(KeyEvent));
      *key_event = clickable_event;
      en = spawn_clickable(w, c, key_event);
      i++;
    }

    // Furnace
    {
      actualise_grid_coordinates(&x, &y, i);
      c = malloc(sizeof(Clickable));
      c->rect = malloc(sizeof(SDL_Rect));
      *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
      c->is_clicked = 0;
      c->click_event = beaver_grid(w, i, get_entity(w, e));
      c->text = malloc(sizeof(Text));
      c->text->str = malloc(1);
      *(c->text->str) = '\0';
      c->text->color = malloc(1);
      c->sprite = malloc(sizeof(Sprite));
      c->sprite->rect = malloc(sizeof(SDL_Rect));
      *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
      c->sprite->texture = get_texture("asset/sprites/furnace.bmp", r, wi);
      key_event = malloc(sizeof(KeyEvent));
      *key_event = clickable_event;
      en = spawn_clickable(w, c, key_event);
      i++;
    }

    // Casern
    {
      actualise_grid_coordinates(&x, &y, i);
      c = malloc(sizeof(Clickable));
      c->rect = malloc(sizeof(SDL_Rect));
      *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
      c->is_clicked = 0;
      c->click_event = beaver_grid(w, i, get_entity(w, e));
      c->text = malloc(sizeof(Text));
      c->text->str = malloc(1);
      *(c->text->str) = '\0';
      c->text->color = malloc(1);
      c->sprite = malloc(sizeof(Sprite));
      c->sprite->rect = malloc(sizeof(SDL_Rect));
      *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 64, .w = 64};
      c->sprite->texture =
          get_texture("asset/sprites/tanuki_casern.bmp", r, wi);
      key_event = malloc(sizeof(KeyEvent));
      *key_event = clickable_event;
      en = spawn_clickable(w, c, key_event);
      i++;
    }
    {
      h = malloc(sizeof(Hoverable));
      h->rect = malloc(sizeof(SDL_Rect));
      *(h->rect) = *(c->rect);
      h->t = CASERN;
      h->text = malloc(sizeof(ActualisedText));
      h->text->color = malloc(sizeof(SDL_Color));
      *(h->text->color) = (SDL_Color){.r = 0, .g = 0, .b = 0, .a = 255};
      h->text->rect = malloc(sizeof(SDL_Rect));
      *(h->text->rect) = hrect;
      h->text->get_text = unit_hover_text;
      ecs_add_component(w, en, COMP_HOVERABLE, h);
    }

    // Tower
    {
      actualise_grid_coordinates(&x, &y, i);
      c = malloc(sizeof(Clickable));
      c->rect = malloc(sizeof(SDL_Rect));
      *(c->rect) = (SDL_Rect){.x = x + 8, .y = y, .h = 32, .w = 16};
      c->is_clicked = 0;
      c->click_event = beaver_grid(w, i, get_entity(w, e));
      c->text = malloc(sizeof(Text));
      c->text->str = malloc(1);
      *(c->text->str) = '\0';
      c->text->color = malloc(1);
      c->sprite = malloc(sizeof(Sprite));
      c->sprite->rect = malloc(sizeof(SDL_Rect));
      *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 64, .w = 32};
      c->sprite->texture = get_texture("asset/sprites/tanuki_tower.bmp", r, wi);
      key_event = malloc(sizeof(KeyEvent));
      *key_event = clickable_event;
      en = spawn_clickable(w, c, key_event);
      i++;
    }

    // Konbini
    {
      actualise_grid_coordinates(&x, &y, i);
      c = malloc(sizeof(Clickable));
      c->rect = malloc(sizeof(SDL_Rect));
      *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
      c->is_clicked = 0;
      c->click_event = beaver_grid(w, i, get_entity(w, e));
      c->text = malloc(sizeof(Text));
      c->text->str = malloc(1);
      *(c->text->str) = '\0';
      c->text->color = malloc(1);
      c->sprite = malloc(sizeof(Sprite));
      c->sprite->rect = malloc(sizeof(SDL_Rect));
      *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 64, .w = 64};
      c->sprite->texture =
          get_texture("asset/sprites/konbini_tanuki.bmp", r, wi);
      key_event = malloc(sizeof(KeyEvent));
      *key_event = clickable_event;
      en = spawn_clickable(w, c, key_event);
      i++;
    }

    // House
    {
      actualise_grid_coordinates(&x, &y, i);
      c = malloc(sizeof(Clickable));
      c->rect = malloc(sizeof(SDL_Rect));
      *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
      c->is_clicked = 0;
      c->click_event = beaver_grid(w, i, get_entity(w, e));
      c->text = malloc(sizeof(Text));
      c->text->str = malloc(1);
      *(c->text->str) = '\0';
      c->text->color = malloc(1);
      c->sprite = malloc(sizeof(Sprite));
      c->sprite->rect = malloc(sizeof(SDL_Rect));
      *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
      c->sprite->texture = get_texture("asset/sprites/tanuki_house.bmp", r, wi);
      key_event = malloc(sizeof(KeyEvent));
      *key_event = clickable_event;
      en = spawn_clickable(w, c, key_event);
      i++;
    }

    // Fort
    {
      actualise_grid_coordinates(&x, &y, i);
      c = malloc(sizeof(Clickable));
      c->rect = malloc(sizeof(SDL_Rect));
      *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
      c->is_clicked = 0;
      c->click_event = beaver_grid(w, i, get_entity(w, e));
      c->text = malloc(sizeof(Text));
      c->text->str = malloc(1);
      *(c->text->str) = '\0';
      c->text->color = malloc(1);
      c->sprite = malloc(sizeof(Sprite));
      c->sprite->rect = malloc(sizeof(SDL_Rect));
      *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 64, .w = 64};
      c->sprite->texture = get_texture("asset/sprites/tanuki_fort.bmp", r, wi);
      key_event = malloc(sizeof(KeyEvent));
      *key_event = clickable_event;
      en = spawn_clickable(w, c, key_event);
      i++;
    }
    break;
  case FORUM:
    // Tanuki
    {
      actualise_grid_coordinates(&x, &y, i);
      c = malloc(sizeof(Clickable));
      c->rect = malloc(sizeof(SDL_Rect));
      *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
      c->is_clicked = 0;
      c->click_event = forum_grid(w, i, get_entity(w, e));
      c->text = malloc(sizeof(Text));
      c->text->str = malloc(1);
      *(c->text->str) = '\0';
      c->text->color = malloc(1);
      c->sprite = malloc(sizeof(Sprite));
      c->sprite->rect = malloc(sizeof(SDL_Rect));
      *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
      c->sprite->texture = get_texture("asset/sprites/tanuki.bmp", r, wi);
      key_event = malloc(sizeof(KeyEvent));
      *key_event = clickable_event;
      en = spawn_clickable(w, c, key_event);
      i++;
    }

    // Beaver
    {
      actualise_grid_coordinates(&x, &y, i);
      c = malloc(sizeof(Clickable));
      c->rect = malloc(sizeof(SDL_Rect));
      *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
      c->is_clicked = 0;
      c->click_event = forum_grid(w, i, get_entity(w, e));
      c->text = malloc(sizeof(Text));
      c->text->str = malloc(1);
      *(c->text->str) = '\0';
      c->text->color = malloc(1);
      c->sprite = malloc(sizeof(Sprite));
      c->sprite->rect = malloc(sizeof(SDL_Rect));
      *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
      c->sprite->texture =
          get_texture("asset/sprites/tanuki_beaver.bmp", r, wi);
      key_event = malloc(sizeof(KeyEvent));
      *key_event = clickable_event;
      en = spawn_clickable(w, c, key_event);
      i++;
    }
    {
      h = malloc(sizeof(Hoverable));
      h->rect = malloc(sizeof(SDL_Rect));
      *(h->rect) = *(c->rect);
      h->t = BEAVER;
      h->text = malloc(sizeof(ActualisedText));
      h->text->color = malloc(sizeof(SDL_Color));
      *(h->text->color) = (SDL_Color){.r = 0, .g = 0, .b = 0, .a = 255};
      h->text->rect = malloc(sizeof(SDL_Rect));
      *(h->text->rect) = hrect;
      h->text->get_text = unit_hover_text;
      ecs_add_component(w, en, COMP_HOVERABLE, h);
    }
    break;

  case DEBUG:
    actualise_grid_coordinates(&x, &y, i);
    c = malloc(sizeof(Clickable));
    c->rect = malloc(sizeof(SDL_Rect));
    *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
    c->is_clicked = 0;
    c->click_event = debug_grid(w, i, get_entity(w, e));
    c->text = malloc(sizeof(Text));
    c->text->str = malloc(1);
    *(c->text->str) = '\0';
    c->text->color = malloc(1);
    c->sprite = malloc(sizeof(Sprite));
    c->sprite->rect = malloc(sizeof(SDL_Rect));
    *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
    c->sprite->texture = get_texture("asset/debug/swp.bmp", r, wi);
    key_event = malloc(sizeof(KeyEvent));
    *key_event = clickable_event;
    en = spawn_clickable(w, c, key_event);
    i++;

    actualise_grid_coordinates(&x, &y, i);
    c = malloc(sizeof(Clickable));
    c->rect = malloc(sizeof(SDL_Rect));
    *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
    c->is_clicked = 0;
    c->click_event = debug_grid(w, i, get_entity(w, e));
    c->text = malloc(sizeof(Text));
    c->text->str = malloc(1);
    *(c->text->str) = '\0';
    c->text->color = malloc(1);
    c->sprite = malloc(sizeof(Sprite));
    c->sprite->rect = malloc(sizeof(SDL_Rect));
    *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
    c->sprite->texture = get_texture("asset/debug/const.bmp", r, wi);
    key_event = malloc(sizeof(KeyEvent));
    *key_event = clickable_event;
    en = spawn_clickable(w, c, key_event);
    i++;

    actualise_grid_coordinates(&x, &y, i);
    c = malloc(sizeof(Clickable));
    c->rect = malloc(sizeof(SDL_Rect));
    *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
    c->is_clicked = 0;
    c->click_event = debug_grid(w, i, get_entity(w, e));
    c->text = malloc(sizeof(Text));
    c->text->str = malloc(1);
    *(c->text->str) = '\0';
    c->text->color = malloc(1);
    c->sprite = malloc(sizeof(Sprite));
    c->sprite->rect = malloc(sizeof(SDL_Rect));
    *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
    c->sprite->texture = get_texture("asset/debug/dam.bmp", r, wi);
    key_event = malloc(sizeof(KeyEvent));
    *key_event = clickable_event;
    en = spawn_clickable(w, c, key_event);
    i++;

    actualise_grid_coordinates(&x, &y, i);
    c = malloc(sizeof(Clickable));
    c->rect = malloc(sizeof(SDL_Rect));
    *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
    c->is_clicked = 0;
    c->click_event = debug_grid(w, i, get_entity(w, e));
    c->text = malloc(sizeof(Text));
    c->text->str = malloc(1);
    *(c->text->str) = '\0';
    c->text->color = malloc(1);
    c->sprite = malloc(sizeof(Sprite));
    c->sprite->rect = malloc(sizeof(SDL_Rect));
    *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
    c->sprite->texture = get_texture("asset/debug/clay.bmp", r, wi);
    key_event = malloc(sizeof(KeyEvent));
    *key_event = clickable_event;
    en = spawn_clickable(w, c, key_event);
    i++;

    actualise_grid_coordinates(&x, &y, i);
    c = malloc(sizeof(Clickable));
    c->rect = malloc(sizeof(SDL_Rect));
    *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
    c->is_clicked = 0;
    c->click_event = debug_grid(w, i, get_entity(w, e));
    c->text = malloc(sizeof(Text));
    c->text->str = malloc(1);
    *(c->text->str) = '\0';
    c->text->color = malloc(1);
    c->sprite = malloc(sizeof(Sprite));
    c->sprite->rect = malloc(sizeof(SDL_Rect));
    *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
    c->sprite->texture = get_texture("asset/debug/water.bmp", r, wi);
    key_event = malloc(sizeof(KeyEvent));
    *key_event = clickable_event;
    en = spawn_clickable(w, c, key_event);
    i++;

    actualise_grid_coordinates(&x, &y, i);
    c = malloc(sizeof(Clickable));
    c->rect = malloc(sizeof(SDL_Rect));
    *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
    c->is_clicked = 0;
    c->click_event = debug_grid(w, i, get_entity(w, e));
    c->text = malloc(sizeof(Text));
    c->text->str = malloc(1);
    *(c->text->str) = '\0';
    c->text->color = malloc(1);
    c->sprite = malloc(sizeof(Sprite));
    c->sprite->rect = malloc(sizeof(SDL_Rect));
    *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
    c->sprite->texture = get_texture("asset/sprites/tanuki_maid.bmp", r, wi);
    key_event = malloc(sizeof(KeyEvent));
    *key_event = clickable_event;
    en = spawn_clickable(w, c, key_event);
    i++;

    actualise_grid_coordinates(&x, &y, i);
    c = malloc(sizeof(Clickable));
    c->rect = malloc(sizeof(SDL_Rect));
    *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
    c->is_clicked = 0;
    c->click_event = debug_grid(w, i, get_entity(w, e));
    c->text = malloc(sizeof(Text));
    c->text->str = malloc(1);
    *(c->text->str) = '\0';
    c->text->color = malloc(1);
    c->sprite = malloc(sizeof(Sprite));
    c->sprite->rect = malloc(sizeof(SDL_Rect));
    *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
    c->sprite->texture = get_texture("asset/sprites/not_a_rhino.bmp", r, wi);
    key_event = malloc(sizeof(KeyEvent));
    *key_event = clickable_event;
    en = spawn_clickable(w, c, key_event);
    i++;
    break;

  case CASERN:

    // Samurai
    actualise_grid_coordinates(&x, &y, i);
    c = malloc(sizeof(Clickable));
    c->rect = malloc(sizeof(SDL_Rect));
    *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
    c->is_clicked = 0;
    c->click_event = casern_grid(w, i, get_entity(w, e));
    c->text = malloc(sizeof(Text));
    c->text->str = malloc(1);
    *(c->text->str) = '\0';
    c->text->color = malloc(1);
    c->sprite = malloc(sizeof(Sprite));
    c->sprite->rect = malloc(sizeof(SDL_Rect));
    *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
    c->sprite->texture = get_texture("asset/sprites/samurai.bmp", r, wi);
    key_event = malloc(sizeof(KeyEvent));
    *key_event = clickable_event;
    en = spawn_clickable(w, c, key_event);
    i++;

    // Security
    actualise_grid_coordinates(&x, &y, i);
    c = malloc(sizeof(Clickable));
    c->rect = malloc(sizeof(SDL_Rect));
    *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
    c->is_clicked = 0;
    c->click_event = casern_grid(w, i, get_entity(w, e));
    c->text = malloc(sizeof(Text));
    c->text->str = malloc(1);
    *(c->text->str) = '\0';
    c->text->color = malloc(1);
    c->sprite = malloc(sizeof(Sprite));
    c->sprite->rect = malloc(sizeof(SDL_Rect));
    *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
    c->sprite->texture = get_texture("asset/sprites/tanuki_secu.bmp", r, wi);
    key_event = malloc(sizeof(KeyEvent));
    *key_event = clickable_event;
    en = spawn_clickable(w, c, key_event);
    i++;

    // Tanuki
    actualise_grid_coordinates(&x, &y, i);
    c = malloc(sizeof(Clickable));
    c->rect = malloc(sizeof(SDL_Rect));
    *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
    c->is_clicked = 0;
    c->click_event = casern_grid(w, i, get_entity(w, e));
    c->text = malloc(sizeof(Text));
    c->text->str = malloc(1);
    *(c->text->str) = '\0';
    c->text->color = malloc(1);
    c->sprite = malloc(sizeof(Sprite));
    c->sprite->rect = malloc(sizeof(SDL_Rect));
    *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
    c->sprite->texture = get_texture("asset/sprites/tanuki.bmp", r, wi);
    key_event = malloc(sizeof(KeyEvent));
    *key_event = clickable_event;
    en = spawn_clickable(w, c, key_event);
    i++;
    break;

  case FORT:

    // T34
    actualise_grid_coordinates(&x, &y, i);
    c = malloc(sizeof(Clickable));
    c->rect = malloc(sizeof(SDL_Rect));
    *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
    c->is_clicked = 0;
    c->click_event = fort_grid(w, i, get_entity(w, e));
    c->text = malloc(sizeof(Text));
    c->text->str = malloc(1);
    *(c->text->str) = '\0';
    c->text->color = malloc(1);
    c->sprite = malloc(sizeof(Sprite));
    c->sprite->rect = malloc(sizeof(SDL_Rect));
    *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
    c->sprite->texture = get_texture("asset/sprites/tanuki_t34.bmp", r, wi);
    key_event = malloc(sizeof(KeyEvent));
    *key_event = clickable_event;
    en = spawn_clickable(w, c, key_event);
    i++;

    // Maid
    actualise_grid_coordinates(&x, &y, i);
    c = malloc(sizeof(Clickable));
    c->rect = malloc(sizeof(SDL_Rect));
    *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
    c->is_clicked = 0;
    c->click_event = fort_grid(w, i, get_entity(w, e));
    c->text = malloc(sizeof(Text));
    c->text->str = malloc(1);
    *(c->text->str) = '\0';
    c->text->color = malloc(1);
    c->sprite = malloc(sizeof(Sprite));
    c->sprite->rect = malloc(sizeof(SDL_Rect));
    *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
    c->sprite->texture = get_texture("asset/sprites/tanuki_maid.bmp", r, wi);
    key_event = malloc(sizeof(KeyEvent));
    *key_event = clickable_event;
    en = spawn_clickable(w, c, key_event);
    i++;

    // Pingu
    actualise_grid_coordinates(&x, &y, i);
    c = malloc(sizeof(Clickable));
    c->rect = malloc(sizeof(SDL_Rect));
    *(c->rect) = (SDL_Rect){.x = x, .y = y, .h = 32, .w = 32};
    c->is_clicked = 0;
    c->click_event = fort_grid(w, i, get_entity(w, e));
    c->text = malloc(sizeof(Text));
    c->text->str = malloc(1);
    *(c->text->str) = '\0';
    c->text->color = malloc(1);
    c->sprite = malloc(sizeof(Sprite));
    c->sprite->rect = malloc(sizeof(SDL_Rect));
    *(c->sprite->rect) = (SDL_Rect){.x = 0, .y = 0, .h = 32, .w = 32};
    c->sprite->texture = get_texture("asset/sprites/santanuki.bmp", r, wi);
    key_event = malloc(sizeof(KeyEvent));
    *key_event = clickable_event;
    en = spawn_clickable(w, c, key_event);
    i++;
    break;

  default:
    break;
  }
}

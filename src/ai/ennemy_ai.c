#include "ennemy_ai.h"
#include "../actionnable.h"
#include "../components.h"
#include "../construction.h"
#include "../data_structures/asset_manager.h"
#include "../pathfinding.h"
#include "../players.h"
#include "../selection.h"
#include "../units/units.h"
#include "steering_behaviors.h"

char is_ai_attacked(World *w) {
  Bitflag flag = COMPF_ACTIONNABLE;
  VEC(EntityRef) es = world_query(w, &flag);
  for (uint i = 0; i < vec_len(es); i++) {
    Entity *e = get_entity(w, es[i]);
    Actionnable *ac = entity_get_component(w, e, COMP_ACTIONNABLE);
    Ownership *ow = entity_get_component(w, e, COMP_OWNERSHIP);
    if (ac->act == Attack && ow->owner == 0) {
      if (entity_get_component(w, get_entity(w, ac->target),
                               COMP_BUILDINGGHOST))
        return 1;
    }
  }
  return 0;
}

void reconsider_ai_state(World *w, AiState *ais) {
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
    // not gonna lie, game ai isn't my thing; I've got no idea if my metrics are
    // relevant, the state machine is clearly overly simplistic (only 3 states,
    // seriously?) and there is no way we will have time to playtest it enough
    // that it gets even remotely good
  case Eco:
    if (is_ai_attacked(w))
      *ais = Defense;
    if ((pm1->dclay * pm1->clay_multiplier +
         pm1->dwater * pm1->water_multiplier) /
            1.2 >
        pm0->dclay * pm0->clay_multiplier + pm0->dwater * pm0->water_multiplier)
      *ais = Offense;
    break;
  case Offense:
    if (is_ai_attacked(w))
      *ais = Defense;
    if (pm1->dclay * pm1->clay_multiplier +
            pm1->dwater * pm1->water_multiplier <
        (pm0->dclay * pm0->clay_multiplier +
         pm0->dwater * pm0->water_multiplier) /
            1.2)
      *ais = Offense;
    break;
  case Defense:
    if (!is_ai_attacked(w))
      *ais = Eco;
    break;
  }
}

void deghost(World *w) {
  // resolve the unused ghost situation :
  Bitflag flag = COMPF_BUILDINGGHOST;
  VEC(EntityRef) *es = world_query_mut(w, &flag);
  for (uint i = 0; i < vec_len(*es); i++) {
    EntityRef eref = (*es)[i];
    Entity *e = get_entity(w, eref);
    BuildingGhost *bg = entity_get_component(w, e, COMP_BUILDINGGHOST);
    if (!bg->construction_done) {
      Ownership *o = entity_get_component(w, e, COMP_OWNERSHIP);
      if (o && o->owner == 1) {
        Bitflag flag = COMPF_UNIT;
        VEC(EntityRef) es2 = world_query(w, &flag);
        char is_ok = 0;

        for (uint i = 0; i < vec_len(es2); i++) {
          Entity *e = get_entity(w, es2[i]);
          Ownership *o = entity_get_component(w, e, COMP_OWNERSHIP);
          if (o->owner == 1) {
            Unit *u = entity_get_component(w, e, COMP_UNIT);
            if (u->t == UBEAVER) {
              Actionnable *act = entity_get_component(w, e, COMP_ACTIONNABLE);
              if (act->act == Build && act->target == eref) {
                is_ok = 1;
                break;
              }
            }
          }
        }

        if (!is_ok) {
          despawn_entity(w, e);
        }
      }
    }
  }
}

void take_ai_action(World *w, AiState *ais, SDL_Renderer *renderer,
                    SDL_Window *window) {
  switch (*ais) {
  case Eco:
    // here we want two builders that build as many wells and furnaces as
    // possible
    {
      Entity *forum = 0;
      VEC(EntityRef) builders = vec_new(EntityRef);
      Bitflag flag = COMPF_UNIT;
      VEC(EntityRef) es = world_query(w, &flag);
      for (uint i = 0; i < vec_len(es); i++) {
        Entity *e = get_entity(w, es[i]);
        Ownership *o = entity_get_component(w, e, COMP_OWNERSHIP);
        if (o->owner == 1) {
          Unit *u = entity_get_component(w, e, COMP_UNIT);
          if (u->t == UFORUM)
            forum = e;
          else if (u->t == UBEAVER) {
            Actionnable *act = entity_get_component(w, e, COMP_ACTIONNABLE);
            if (act->act == Lazy)
              vec_push(builders, es[i]);
          }
        }
      }
      if (!forum) {
        vec_free(builders);
        return;
      }
      flag = COMPF_PLAYERMANAGER;
      VEC(EntityRef) ps = world_query(w, &flag);
      PlayerManager *pm0 =
          entity_get_component(w, get_entity(w, ps[0]), COMP_PLAYERMANAGER);
      PlayerManager *pm1 =
          entity_get_component(w, get_entity(w, ps[1]), COMP_PLAYERMANAGER);
      if (pm0->id == 1) {
        pm1 = pm0;
      }

      if (vec_len(builders) < 2) {
        Position *p = entity_get_component(w, forum, COMP_POSITION);
        int i = 0;
        while (pm1->water > 100 && i++ < 2) {
          pm1->water -= 100;
          spawn_unit(w, UBEAVER, renderer, window,
                     (Position){p->x + (float)(rand() % 2000 - 1000) / 10,
                                p->y + (float)(rand() % 2000 - 1000) / 10},
                     1);
        }
      } else {
        flag = COMPF_PLAYERMANAGER;
        for (uint i = 0;
             i < vec_len(builders) && pm1->clay > 200 && pm1->water > 100;
             i += 2) {
          pm1->clay -= 200;
          pm1->water -= 100;
          Position *p = entity_get_component(w, get_entity(w, builders[i]),
                                             COMP_POSITION);
          // Hardcoding is good, actually
          Position target = {1536 + (rand() % (16 * 8)) - 16 * 4,
                             832 + (rand() % (16 * 8)) - 16 * 4};

          Bitflag bf = COMPF_MAPCOMPONENT;
          VEC(EntityRef) mapv = world_query(w, &bf);
          Entity *emap = get_entity(w, mapv[0]);
          MapComponent *mapc = entity_get_component(w, emap, COMP_MAPCOMPONENT);

          Vec2 p_vec2 = (Vec2){.x = p->x, .y = p->y};
          TilePosition tpstart = pos2tile(&p_vec2);

          Vec2 mp_vec2 = (Vec2){.x = target.x, .y = target.y};
          TilePosition tpend = pos2tile(&mp_vec2);

          SteerManager *stm = entity_get_component(
              w, get_entity(w, builders[i]), COMP_STEERMANAGER);

          Path pa = pathfind_astar(mapc->map, UBEAVER, &tpstart, &tpend);
          if (pa) {
            if (vec_len(pa) > 1) {
              free(pa[0]);
              vec_remove(pa, 0);
            }
            if (vec_len(pa) > 1) {
              free(pa[0]);
              vec_remove(pa, 0);
            }
            stm->current_path = pa;
            stm->max_speed = 5.;
            stm->max_force = INFINITY;
          }

          bf = COMPF_ACTIONNABLE;
          Actionnable *act = entity_get_component(w, get_entity(w, builders[i]),
                                                  COMP_ACTIONNABLE);

          {
            Entity *e = spawn_entity(w);
            Ownership *o = calloc(1, sizeof(Ownership));
            o->owner = 1;
            ecs_add_component(w, e, COMP_OWNERSHIP, o);
            UnitT *u = get_unit(UFURNACE, renderer, window);
            BuildingGhost *bg = malloc(sizeof(BuildingGhost));
            *bg = (BuildingGhost){u, 0, u->hp, 0, UFURNACE};
            ecs_add_component(w, e, COMP_BUILDINGGHOST, bg);

            Sprite *sp = malloc(sizeof(Sprite));
            sp->rect = malloc(sizeof(SDL_Rect));
            *(sp->rect) = *(u->sprite->rect);
            sp->texture = u->sprite->texture;

            ecs_add_component(w, e, COMP_SPRITE, sp);
            Position *p = calloc(1, sizeof(Position));
            *p = (Position){target.x, target.y};
            ecs_add_component(w, e, COMP_POSITION, p);
            Selectable *sl = malloc(sizeof(Selectable));
            ecs_add_component(w, e, COMP_SELECTABLE, sl);

            act->act = Build;
            act->target = e->id;
          }
        }
        for (uint i = 1; i < vec_len(builders) && pm1->clay > 300; i += 2) {
          pm1->clay -= 300;
          Position *ps = entity_get_component(w, get_entity(w, builders[i]),
                                              COMP_POSITION);
          Actionnable *act = entity_get_component(w, get_entity(w, builders[i]),
                                                  COMP_ACTIONNABLE);

          {
            Entity *e = spawn_entity(w);
            Ownership *o = calloc(1, sizeof(Ownership));
            o->owner = 1;
            ecs_add_component(w, e, COMP_OWNERSHIP, o);
            UnitT *u = get_unit(UWELL, renderer, window);
            BuildingGhost *bg = malloc(sizeof(BuildingGhost));
            *bg = (BuildingGhost){u, 0, u->hp, 0, UWELL};
            ecs_add_component(w, e, COMP_BUILDINGGHOST, bg);

            Sprite *sp = malloc(sizeof(Sprite));
            sp->rect = malloc(sizeof(SDL_Rect));
            *(sp->rect) = *(u->sprite->rect);
            sp->texture = u->sprite->texture;

            ecs_add_component(w, e, COMP_SPRITE, sp);
            Position *p = calloc(1, sizeof(Position));
            *p = (Position){ps->x + (float)(rand() % 200 - 100) / 10,
                            ps->y + (float)(rand() % 200 - 100) / 10};
            ecs_add_component(w, e, COMP_POSITION, p);
            Selectable *sl = malloc(sizeof(Selectable));
            ecs_add_component(w, e, COMP_SELECTABLE, sl);

            act->act = Build;
            act->target = e->id;
          }
        }
      }

      vec_free(builders);
    }
    break;
  /*
   *
   *
   *   Don't mind me, I'm just spacing
   *
   *
   *
   */
  case Offense:
    // here we want two barracks and as many units as possible, + attacks one
    // the ennemy base
    {

      Entity *forum = 0;
      VEC(EntityRef) builders = vec_new(EntityRef);
      VEC(EntityRef) barracks = vec_new(EntityRef);
      VEC(EntityRef) fighters = vec_new(EntityRef);
      VEC(EntityRef) targets = vec_new(EntityRef);
      int builder_count = 0;

      Bitflag flag = COMPF_UNIT;
      VEC(EntityRef) es = world_query(w, &flag);
      for (uint i = 0; i < vec_len(es); i++) {
        Entity *e = get_entity(w, es[i]);
        Ownership *o = entity_get_component(w, e, COMP_OWNERSHIP);
        if (o->owner == 1) {
          Unit *u = entity_get_component(w, e, COMP_UNIT);
          if (u->t == UFORUM)
            forum = e;
          else if (u->t == UBEAVER) {
            builder_count++;
            Actionnable *act = entity_get_component(w, e, COMP_ACTIONNABLE);
            if (act->act == Lazy)
              vec_push(builders, es[i]);
          } else if (u->t == UCASERN) {
            vec_push(barracks, es[i]);
          } else if (u->b_dam != 0 || u->p_dam != 0 || u->s_dam != 0) {
            vec_push(fighters, es[i]);
          }
        } else {
          vec_push(targets, es[i]);
        }
      }
      if (!forum) {
        vec_free(builders);
        vec_free(barracks);
        vec_free(fighters);
        vec_free(targets);
        return;
      }
      flag = COMPF_PLAYERMANAGER;
      VEC(EntityRef) ps = world_query(w, &flag);
      PlayerManager *pm0 =
          entity_get_component(w, get_entity(w, ps[0]), COMP_PLAYERMANAGER);
      PlayerManager *pm1 =
          entity_get_component(w, get_entity(w, ps[1]), COMP_PLAYERMANAGER);
      if (pm0->id == 1) {
        pm1 = pm0;
      }

      if (builder_count < 2) {
        Position *p = entity_get_component(w, forum, COMP_POSITION);
        int i = 0;
        while (pm1->water > 100 && i++ < 2) {
          pm1->water -= 100;
          spawn_unit(w, UBEAVER, renderer, window,
                     (Position){p->x + (float)(rand() % 2000 - 1000) / 10,
                                p->y + (float)(rand() % 2000 - 1000) / 10},
                     1);
        }
      } else if (vec_len(barracks) < 2) {
        for (uint i = 1; i < vec_len(builders) && i < 2 && pm1->clay > 600 &&
                         pm1->water > 400;
             i += 2) {
          pm1->clay -= 600;
          pm1->water -= 400;

          Position *ps = entity_get_component(w, get_entity(w, builders[i]),
                                              COMP_POSITION);
          Actionnable *act = entity_get_component(w, get_entity(w, builders[i]),
                                                  COMP_ACTIONNABLE);

          {
            Entity *e = spawn_entity(w);
            Ownership *o = calloc(1, sizeof(Ownership));
            o->owner = 1;
            ecs_add_component(w, e, COMP_OWNERSHIP, o);
            UnitT *u = get_unit(UCASERN, renderer, window);
            BuildingGhost *bg = malloc(sizeof(BuildingGhost));
            *bg = (BuildingGhost){u, 0, u->hp, 0, UCASERN};
            ecs_add_component(w, e, COMP_BUILDINGGHOST, bg);

            Sprite *sp = malloc(sizeof(Sprite));
            sp->rect = malloc(sizeof(SDL_Rect));
            *(sp->rect) = *(u->sprite->rect);
            sp->texture = u->sprite->texture;

            ecs_add_component(w, e, COMP_SPRITE, sp);
            Position *p = calloc(1, sizeof(Position));
            *p = (Position){ps->x + (float)(rand() % 200 - 100) / 10,
                            ps->y + (float)(rand() % 200 - 100) / 10};
            ecs_add_component(w, e, COMP_POSITION, p);
            Selectable *sl = malloc(sizeof(Selectable));
            ecs_add_component(w, e, COMP_SELECTABLE, sl);

            act->act = Build;
            act->target = e->id;
          }
        }
      } else {
        for (uint i = 0;
             i < vec_len(barracks) && pm1->water > 80 && pm1->clay > 40; i++) {
          pm1->clay -= 600;
          pm1->water -= 400;
          Position *ps = entity_get_component(w, get_entity(w, barracks[i]),
                                              COMP_POSITION);
          spawn_unit(w, BASE_FISH, renderer, window,
                     (Position){ps->x + (float)(rand() % 2000 - 1000) / 10,
                                ps->y + (float)(rand() % 2000 - 1000) / 10},
                     1);
        }
      }

      for (uint i = 0; i < vec_len(fighters); i++) {
        Actionnable *act = entity_get_component(w, get_entity(w, fighters[i]),
                                                COMP_ACTIONNABLE);
        if (act->act == Lazy) {
          if (!vec_len(targets)) {
            fprintf(stderr, "somehow the player has nothing anymore\n");
            return;
          }
          int targ = rand() % vec_len(targets);
          act->act = Attack;
          act->target = targets[targ];
        }
      }

      vec_free(builders);
      vec_free(barracks);
      vec_free(fighters);
      vec_free(targets);
    }

    break;
  /*
   *
   *
   *   Don't mind me, I'm just spacing
   *
   *
   *
   */
  case Defense:
    // here we panic and send everything we have to whatever is attacking (the
    // buildings) and try to get more units with whatever we still have
    {

      Entity *forum = 0;
      VEC(EntityRef) builders = vec_new(EntityRef);
      VEC(EntityRef) barracks = vec_new(EntityRef);
      VEC(EntityRef) fighters = vec_new(EntityRef);
      VEC(EntityRef) targets = vec_new(EntityRef);
      int builder_count = 0;

      Bitflag flag = COMPF_UNIT;
      VEC(EntityRef) es = world_query(w, &flag);
      for (uint i = 0; i < vec_len(es); i++) {
        Entity *e = get_entity(w, es[i]);
        Ownership *o = entity_get_component(w, e, COMP_OWNERSHIP);
        if (o->owner == 1) {
          Unit *u = entity_get_component(w, e, COMP_UNIT);
          if (u->t == UFORUM)
            forum = e;
          else if (u->t == UBEAVER) {
            builder_count++;
            Actionnable *act = entity_get_component(w, e, COMP_ACTIONNABLE);
            if (act->act == Lazy)
              vec_push(builders, es[i]);
          } else if (u->t == UCASERN) {
            vec_push(barracks, es[i]);
          } else if (u->b_dam != 0 || u->p_dam != 0 || u->s_dam != 0) {
            vec_push(fighters, es[i]);
          }
        } else {
          Actionnable *a = entity_get_component(w, e, COMP_ACTIONNABLE);
          if (a && a->act == Attack)
            vec_push(targets, es[i]);
        }
      }
      if (!forum) {
        vec_free(builders);
        vec_free(barracks);
        vec_free(fighters);
        vec_free(targets);
        return;
      }

      flag = COMPF_PLAYERMANAGER;
      VEC(EntityRef) ps = world_query(w, &flag);
      PlayerManager *pm0 =
          entity_get_component(w, get_entity(w, ps[0]), COMP_PLAYERMANAGER);
      PlayerManager *pm1 =
          entity_get_component(w, get_entity(w, ps[1]), COMP_PLAYERMANAGER);
      if (pm0->id == 1) {
        pm1 = pm0;
      }

      if (builder_count < 2) {
        Position *p = entity_get_component(w, forum, COMP_POSITION);
        int i = 0;
        while (pm1->water > 100 && i++ < 2) {
          pm1->water -= 100;
          spawn_unit(w, UBEAVER, renderer, window, (Position){p->x, p->y}, 1);
        }
      } else if (vec_len(barracks) < 2) {
        for (uint i = 1; i < vec_len(builders) && i < 2 && pm1->clay > 600 &&
                         pm1->water > 400;
             i += 2) {
          pm1->clay -= 600;
          pm1->water -= 400;

          Position *ps = entity_get_component(w, get_entity(w, builders[i]),
                                              COMP_POSITION);
          Actionnable *act = entity_get_component(w, get_entity(w, builders[i]),
                                                  COMP_ACTIONNABLE);

          {
            Entity *e = spawn_entity(w);
            Ownership *o = calloc(1, sizeof(Ownership));
            o->owner = 1;
            ecs_add_component(w, e, COMP_OWNERSHIP, o);
            UnitT *u = get_unit(UCASERN, renderer, window);
            BuildingGhost *bg = malloc(sizeof(BuildingGhost));
            *bg = (BuildingGhost){u, 0, u->hp, 0, UCASERN};
            ecs_add_component(w, e, COMP_BUILDINGGHOST, bg);

            Sprite *sp = malloc(sizeof(Sprite));
            sp->rect = malloc(sizeof(SDL_Rect));
            *(sp->rect) = *(u->sprite->rect);
            sp->texture = u->sprite->texture;

            ecs_add_component(w, e, COMP_SPRITE, sp);
            Position *p = calloc(1, sizeof(Position));
            *p = (Position){ps->x + (float)(rand() % 200 - 100) / 10,
                            ps->y + (float)(rand() % 200 - 100) / 10};
            ecs_add_component(w, e, COMP_POSITION, p);
            Selectable *sl = malloc(sizeof(Selectable));
            ecs_add_component(w, e, COMP_SELECTABLE, sl);

            act->act = Build;
            act->target = e->id;
          }
        }
      } else {
        for (uint i = 0;
             i < vec_len(barracks) && pm1->water > 80 && pm1->clay > 40; i++) {
          pm1->clay -= 600;
          pm1->water -= 400;
          Position *ps = entity_get_component(w, get_entity(w, barracks[i]),
                                              COMP_POSITION);
          spawn_unit(w, BASE_FISH, renderer, window,
                     (Position){ps->x + (float)(rand() % 2000 - 1000) / 10,
                                ps->y + (float)(rand() % 2000 - 1000) / 10},
                     1);
        }
      }

      for (uint i = 0; i < vec_len(fighters); i++) {
        Actionnable *act = entity_get_component(w, get_entity(w, fighters[i]),
                                                COMP_ACTIONNABLE);
        if (act->act == Lazy) {
          if (!vec_len(targets)) {
            fprintf(stderr, "somehow the player has nothing anymore\n");
            return;
          }
          int targ = rand() % vec_len(targets);
          act->act = Attack;
          act->target = targets[targ];
        }
      }

      vec_free(builders);
      vec_free(barracks);
      vec_free(fighters);
      vec_free(targets);
    }
    break;
  }
}

void ai_defends_itself(World *w) {
  Bitflag flag = COMPF_ACTIONNABLE;
  VEC(EntityRef) es = world_query(w, &flag);
  for (uint i = 0; i < vec_len(es); i++) {
    Actionnable *act =
        entity_get_component(w, get_entity(w, es[i]), COMP_ACTIONNABLE);
    SteerManager *stm =
        entity_get_component(w, get_entity(w, es[i]), COMP_STEERMANAGER);
    Ownership *o =
        entity_get_component(w, get_entity(w, es[i]), COMP_OWNERSHIP);
    if (o->owner == 0 && act->act == Attack &&
        (!stm->current_path || !vec_len(stm->current_path))) {
      Entity *e = get_entity(w, act->target);
      Actionnable *act2 = entity_get_component(w, e, COMP_ACTIONNABLE);
      if (act2 && act2->act != Attack) {
        act2->act = Attack;
        act2->target = es[i];
      }
    }
  }
}

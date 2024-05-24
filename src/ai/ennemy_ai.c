#include "ennemy_ai.h"
#include "../actionnable.h"
#include "../components.h"
#include "../construction.h"
#include "../data_structures/asset_manager.h"
#include "../pathfinding.h"
#include "../players.h"
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
    if (pm1->dclay + pm1->dwater * 1.2 > pm0->dclay + pm0->dwater)
      *ais = Offense;
    break;
  case Offense:
    if (is_ai_attacked(w))
      *ais = Defense;
    if (pm1->dclay + pm1->dwater < 1.2 * (pm0->dclay + pm0->dwater))
      *ais = Offense;
    break;
  case Defense:
    if (!is_ai_attacked(w))
      *ais = Eco;
    break;
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
      VEC(Entity *) builders = vec_new(Entity *);
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
              vec_push(builders, e);
          }
        }
      }
      if (!forum) {
        vec_free(builders);
        return;
      }
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
                     (Position){p->x + (float)(rand() % 200 - 100) / 10,
                                p->y + (float)(rand() % 200 - 100) / 10},
                     1);
        }
      } else {
        flag = COMPF_PLAYERMANAGER;
        for (uint i = 0;
             i < vec_len(builders) && pm1->clay > 200 && pm1->water > 100;
             i += 2) {
          pm1->clay -= 200;
          pm1->water -= 100;
          Position *p = entity_get_component(w, builders[i], COMP_POSITION);
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
              w, get_entity(w, world_query(w, &bf)[0]), COMP_STEERMANAGER);

          Path pa = pathfind_astar(mapc->map, BEAVER, &tpstart, &tpend);
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
          Actionnable *act =
              entity_get_component(w, builders[i], COMP_ACTIONNABLE);

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
            Bitflag bf = COMPF_CAMERA;
            VEC(EntityRef) camv = world_query(w, &bf);
            Entity *ecam = get_entity(w, camv[0]);
            Camera *cam = entity_get_component(w, ecam, COMP_CAMERA);
            Position pworld = screen2worldspace(p, cam);
            *p = pworld;
            ecs_add_component(w, e, COMP_POSITION, p);
            ClaySource *cs = malloc(sizeof(ClaySource));
            ecs_add_component(w, e, COMP_CLAYSOURCE, cs);

            act->act = Build;
            // yes, this is the ref of the entity we just created
            act->target = vec_len(w->entities) - 1;
          }
        }
        for (uint i = 1; i < vec_len(builders) && pm1->clay > 300; i += 2) {
          pm1->clay -= 300;
          Position *ps = entity_get_component(w, builders[i], COMP_POSITION);
          Actionnable *act =
              entity_get_component(w, builders[i], COMP_ACTIONNABLE);

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
            Bitflag bf = COMPF_CAMERA;
            VEC(EntityRef) camv = world_query(w, &bf);
            Entity *ecam = get_entity(w, camv[0]);
            Camera *cam = entity_get_component(w, ecam, COMP_CAMERA);
            Position pworld = screen2worldspace(p, cam);
            *p = pworld;
            ecs_add_component(w, e, COMP_POSITION, p);
            ClaySource *cs = malloc(sizeof(WaterSource));
            ecs_add_component(w, e, COMP_WATERSOURCE, cs);

            act->act = Build;
            // yes, this is the ref of the entity we just created
            act->target = vec_len(w->entities) - 1;
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
      VEC(Entity *) builders = vec_new(Entity *);
      VEC(Entity *) barracks = vec_new(Entity *);
      VEC(Entity *) fighters = vec_new(Entity *);
      VEC(EntityRef) targets = vec_new(EntityRef);

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
              vec_push(builders, e);
          } else if (u->t == UCASERN) {
            vec_push(barracks, e);
          } else if (u->b_dam != 0 || u->p_dam != 0 || u->s_dam != 0) {
            vec_push(fighters, e);
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
                     (Position){p->x + (float)(rand() % 200 - 100) / 10,
                                p->y + (float)(rand() % 200 - 100) / 10},
                     1);
        }
      } else if (vec_len(barracks) < 2) {
        for (uint i = 1; i < vec_len(builders) && i < 2 && pm1->clay > 600 &&
                         pm1->water > 400;
             i += 2) {
          pm1->clay -= 600;
          pm1->water -= 400;

          Position *ps = entity_get_component(w, builders[i], COMP_POSITION);
          Actionnable *act =
              entity_get_component(w, builders[i], COMP_ACTIONNABLE);

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
            Bitflag bf = COMPF_CAMERA;
            VEC(EntityRef) camv = world_query(w, &bf);
            Entity *ecam = get_entity(w, camv[0]);
            Camera *cam = entity_get_component(w, ecam, COMP_CAMERA);
            Position pworld = screen2worldspace(p, cam);
            *p = pworld;
            ecs_add_component(w, e, COMP_POSITION, p);

            act->act = Build;
            // yes, this is the ref of the entity we just created
            act->target = vec_len(w->entities) - 1;
          }
        }
      } else {
        for (uint i = 0;
             i < vec_len(barracks) && pm1->water > 80 && pm1->clay > 40; i++) {
          pm1->clay -= 600;
          pm1->water -= 400;
          Position *ps = entity_get_component(w, barracks[i], COMP_POSITION);
          spawn_unit(w, BASE_FISH, renderer, window,
                     (Position){ps->x + (float)(rand() % 200 - 100) / 10,
                                ps->y + (float)(rand() % 200 - 100) / 10},
                     1);
        }
      }

      for (uint i = 0; i < vec_len(fighters); i++) {
        Actionnable *act =
            entity_get_component(w, builders[i], COMP_ACTIONNABLE);
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
      VEC(Entity *) builders = vec_new(Entity *);
      VEC(Entity *) barracks = vec_new(Entity *);
      VEC(Entity *) fighters = vec_new(Entity *);
      VEC(EntityRef) targets = vec_new(EntityRef);

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
              vec_push(builders, e);
          } else if (u->t == UCASERN) {
            vec_push(barracks, e);
          } else if (u->b_dam != 0 || u->p_dam != 0 || u->s_dam != 0) {
            vec_push(fighters, e);
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
          spawn_unit(w, UBEAVER, renderer, window, (Position){p->x, p->y}, 1);
        }
      } else if (vec_len(barracks) < 2) {
        for (uint i = 1; i < vec_len(builders) && i < 2 && pm1->clay > 600 &&
                         pm1->water > 400;
             i += 2) {
          pm1->clay -= 600;
          pm1->water -= 400;

          Position *ps = entity_get_component(w, builders[i], COMP_POSITION);
          Actionnable *act =
              entity_get_component(w, builders[i], COMP_ACTIONNABLE);

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
            Bitflag bf = COMPF_CAMERA;
            VEC(EntityRef) camv = world_query(w, &bf);
            Entity *ecam = get_entity(w, camv[0]);
            Camera *cam = entity_get_component(w, ecam, COMP_CAMERA);
            Position pworld = screen2worldspace(p, cam);
            *p = pworld;
            ecs_add_component(w, e, COMP_POSITION, p);

            act->act = Build;
            // yes, this is the ref of the entity we just created
            act->target = vec_len(w->entities) - 1;
          }
        }
      } else {
        for (uint i = 0;
             i < vec_len(barracks) && pm1->water > 80 && pm1->clay > 40; i++) {
          pm1->clay -= 600;
          pm1->water -= 400;
          Position *ps = entity_get_component(w, barracks[i], COMP_POSITION);
          spawn_unit(w, BASE_FISH, renderer, window,
                     (Position){ps->x + (float)(rand() % 200 - 100) / 10,
                                ps->y + (float)(rand() % 200 - 100) / 10},
                     1);
        }
      }

      for (uint i = 0; i < vec_len(fighters); i++) {
        Actionnable *act =
            entity_get_component(w, builders[i], COMP_ACTIONNABLE);
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
    if (act->act == Attack) {
      Entity *e = get_entity(w, act->target);
      Actionnable *act2 = entity_get_component(w, e, COMP_ACTIONNABLE);
      if (act2->act != Attack) {
        act2->act = Attack;
        act2->target = es[i];
      }
    }
  }
}

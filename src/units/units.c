#include <stdlib.h>

#include "../actionnable.h"
#include "../ai/steering_behaviors.h"
#include "../components.h"
#include "../data_structures/asset_manager.h"
#include "../renderer/anim.h"
#include "../renderer/sprite.h"
#include "../selection.h"
#include "units.h"

// typedef enum {
//   UNIT_TEST, // funny name :)
//   WELL,
//   BASE_SOLDIER,
//   BASE_FISH,
//   BASE_FLYING,
//   UNIT_NUMBER
// } UnitTypes;

// typedef enum {
//   TILE_PLAIN,
//   TILE_FOREST,
//   TILE_SWAMP,
//   TILE_WATER,
//   TILE_MOUNTAIN,
//   TILE_GIGEMENT,

//   TILE_NUMBER
// } TileTypes;

double units_get_tile_speed(UnitTypes u, TileTypes t) {
  double unit_speeds[UNIT_NUMBER][TILE_NUMBER] = {
  {1., 1., 1., 0., 0., 1.},
  {1,1,1,0,0,1},
  {1., 1., 1., 0., 0., 1.},
  {0.75, 0.75, 0.75, 1.25, 0., 0.75},
  {1., 1., 1., 1., 1., 1.},
  };
  return unit_speeds[u][t];
}

void unit_component_free(void *temp) {
  Unit *unit = (Unit *)temp;
  // free(unit->descr);
  // free(unit->name);
  // the sprite shouldn't be freed as it also has to be its own component
  // sprite_component_free(unit->sprite);
  // free(unit->path_to_sprite)
  free(unit);
}

void unitt_free(UnitT *unit) {
  free(unit->descr);
  free(unit->name);
  sprite_component_free(unit->sprite);
  free(unit->path_to_sprite);
  free(unit);
}

Entity *spawn_unit(World *w, UnitTypes t, SDL_Renderer *renderer,
                   SDL_Window *window, Position p, char owner) {
  Entity *e = spawn_entity(w);
  UnitT *ut = get_unit(t, renderer, window);
  Sprite *s = malloc(sizeof(Sprite));
  s->rect = malloc(sizeof(SDL_Rect));
  *(s->rect) = *(ut->sprite->rect);
  s->texture = ut->sprite->texture;
  Unit *u = malloc(sizeof(Unit));
  *u = (Unit){.b_dam = ut->b_dam,
              .b_def = ut->b_def,
              .descr = ut->descr,
              .hp = ut->hp,
              .max_hp = ut->hp,
              .name = ut->name,
              .p_dam = ut->p_dam,
              .p_def = ut->p_def,
              .path_to_sprite = ut->path_to_sprite,
              .rg = ut->rg,
              .s_dam = ut->s_dam,
              .s_def = ut->s_def,
              .sp = ut->sp,
              .sprite = s,
              .t = t};

  Actionnable *ac = malloc(sizeof(Actionnable));
  *ac = (Actionnable){Lazy, UINT64_MAX};
  ecs_add_component(w, e, COMP_ACTIONNABLE, ac);
  Ownership *o = malloc(sizeof(Ownership));
  *o = (Ownership){owner};
  ecs_add_component(w, e, COMP_OWNERSHIP, o);
  Position *pp = calloc(1, sizeof(Position));
  *pp = p;
  ecs_add_component(w, e, COMP_POSITION, pp);
  ecs_add_component(w, e, COMP_SPRITE, u->sprite);
  ecs_add_component(w, e, COMP_UNIT, u);
  Selectable *se = calloc(1, sizeof(Selectable));
  ecs_add_component(w, e, COMP_SELECTABLE, se);
  Animator *a = malloc(sizeof(Animator));
  *a = animator_new(u);
  ecs_add_component(w, e, COMP_ANIMATOR, a);

  SteerManager *stm = malloc(sizeof(SteerManager));
  switch (t) {

  default:
    *stm = (SteerManager){
        10, 10, 10, 10, 10, 0, (Vec2){0, 0}, (Vec2){p.x, p.y}, (Vec2){0, 0}, 0};
    break;
  }

  ecs_add_component(w, e, COMP_STEERMANAGER, stm);
  return e;
}

// Entity *spawn_ghost(World *w, Selector *s, SDL_Renderer *r, SDL_Window
// *window,
//                     SDL_Point pt) {
//   Entity *e = spawn_entity(w);
//   UnitT *u = parse(s->building, r, window);
//   // ecs_add_component(w, e, COMP_UNIT, u);
//   BuildingGhost *bg = malloc(sizeof(BuildingGhost));
//   *bg = (BuildingGhost){u, 0, u->hp, 0};
//   ecs_add_component(w, e, COMP_BUILDINGGHOST, bg);
//   ecs_add_component(w, e, COMP_SPRITE, u->sprite);
//   Position *p = calloc(1, sizeof(Position));
//   *p = (Position){pt.x, pt.y};
//   ecs_add_component(w, e, COMP_POSITION, p);
//   Selectable *sa = calloc(1, sizeof(Selectable));
//   sa->is_ghost = 1;
//   ecs_add_component(w, e, COMP_SELECTABLE, s);
// }

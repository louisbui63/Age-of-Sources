#include "units.h"
#include "../actionnable.h"
#include "../ai/steering_behaviors.h"
#include "../components.h"
#include "../data_structures/asset_manager.h"
#include "../renderer/anim.h"
#include "../renderer/sprite.h"
#include "../renderer/ui.h"
#include "../selection.h"
#include <stdio.h>
#include <stdlib.h>

// typedef enum {
//   UNIT_TEST, // funny name :)
//   WELL,
//   FURNACE,
//   CASERN,
//   TOWER,
//   KONBINI,
//   HOUSE,
//   FORT,
//   FORUM,
//   UWELL,
//   UFURNACE,
//   UCASERN,
//   UTOWER,
//   UKONBINI,
//   UHOUSE,
//   UFORT,
//   UFORUM,
//   BEAVER,
//   UBEAVER,
//   BASE_SOLDIER,
//   BASE_FISH,
//   MAID,
//   FROG,
//   SHRIMP,
//   HIPPO,
//   NARVAL,
//   PINGU,
//   SAMURAI,
//   SECU,
//   T34,
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
      {1., 1., 1., 0., 0., 1.},           // UNIT_TEST
      {1, 1, 1, 0, 0, 1},                 // WELL
      {0, 0, 0, 0, 0, 1},                 // FURNACE
      {1, 1, 1, 0, 0, 1},                 // CASERN
      {1, 1, 1, 0, 0, 1},                 // TOWER
      {1, 1, 1, 0, 0, 1},                 // KONBINI
      {1, 1, 1, 0, 0, 1},                 // HOUSE
      {1, 1, 1, 0, 0, 1},                 // FORT
      {1, 1, 1, 0, 0, 1},                 // FORUM
      {1, 1, 1, 0, 0, 1},                 // UWELL
      {0, 0, 0, 0, 0, 1},                 // UFURNACE
      {1, 1, 1, 0, 0, 1},                 // UCASERN
      {1, 1, 1, 0, 0, 1},                 // UTOWER
      {1, 1, 1, 0, 0, 1},                 // UKONBINI
      {1, 1, 1, 0, 0, 1},                 // UHOUSE
      {1, 1, 1, 0, 0, 1},                 // UFORT
      {1, 1, 1, 0, 0, 1},                 // UFORUM
      {1., 1., 1., 0., 0., 1.},           // BEAVER
      {1., 1., 1., 0., 0., 1.},           // UBEAVER
      {1., 1., 1., 0., 0., 1.},           // BASE_SOLDIER
      {0.75, 0.75, 0.75, 1.25, 0., 0.75}, // BASE_FISH
      {1., 1., 1., 1., 1., 1.},           // MAID
      {1., 1., 1., 0., 0., 1.},           // FROG
      {1., 1., 1., 0., 0., 1.},           // SHRIMP
      {1, 1, 1, 0.75, 0., 1.},            // HIPPO
      {1., 1., 1., 0., 0., 1.},           // NARVAL
      {1., 1., 1., 1.25, 0., 1.},         // PINGU
      {1., 1., 1., 0., 0., 1.},           // SAMURAI
      {1., 1., 1., 0., 0., 1.},           // SECU
      {1., 1., 1., 0., 0., 1.},           // T34
      {1., 1., 1., 1., 1., 1.},           // DEBUG
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
  // printf("%s\n", ut->path_to_sprite);
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
  // SteerObstacle *st = malloc(sizeof(SteerObstacle));
  // *st =
  //     (SteerObstacle){.bounding_circle = 10, .position = (Vec2){pp->x,
  //     pp->y}};
  // ecs_add_component(w, e, COMP_STEEROBSTACLE, st);

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

// VEC(Clickable *) unit_actions(UnitTypes t) {
//   VEC(Clickable *) vec = vec_new(Clickable *);
//   switch (t) {
//   case BUILDER_TANUKI:
//     Clickable *c = malloc(sizeof(Clickable));
//     c->click_event
//     break;

//   default:
//     break;
//   }
//   return vec;
// }

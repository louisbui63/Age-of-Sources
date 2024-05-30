#include "game_manager.h"
#include "actionnable.h"
#include "components.h"
#include "construction.h"
#include "data_structures/asset_manager.h"
#include "data_structures/map.h"
#include "players.h"
#include "selection.h"
#include "units/units.h"

void new_game(World *w, SDL_Renderer *renderer, SDL_Window *window, Camera *cam,
              AiState *ais) {
  // generate the map from a bmp
  Entity *map = spawn_entity(w);
  MapComponent *mc = malloc(sizeof(MapComponent));
  *mc = (MapComponent){load_map_from_bmp("asset/map.bmp")};
  ecs_add_component(w, map, COMP_MAPCOMPONENT, mc);

  cam->x = 70 * 16;
  cam->y = 90 * 16;

  *ais = Eco;

  // spawn stuff
  { // spawn tanuki forum
    Entity *e = spawn_entity(w);
    UnitT *ut = get_unit(FORUM, renderer, window);
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
                .t = FORUM};

    Actionnable *ac = malloc(sizeof(Actionnable));
    *ac = (Actionnable){Lazy, UINT64_MAX};
    ecs_add_component(w, e, COMP_ACTIONNABLE, ac);
    Ownership *o = malloc(sizeof(Ownership));
    *o = (Ownership){0};
    ecs_add_component(w, e, COMP_OWNERSHIP, o);
    Position *pp = calloc(1, sizeof(Position));
    *pp = (Position){80 * 16, 100 * 16};
    ecs_add_component(w, e, COMP_POSITION, pp);
    ecs_add_component(w, e, COMP_SPRITE, u->sprite);
    ecs_add_component(w, e, COMP_UNIT, u);
    Selectable *se = calloc(1, sizeof(Selectable));
    ecs_add_component(w, e, COMP_SELECTABLE, se);
    BuildingGhost *bg = malloc(sizeof(BuildingGhost));
    *bg = (BuildingGhost){
        ut, 1000, 500, 1, FORUM,
    };
    ecs_add_component(w, e, COMP_BUILDINGGHOST, bg);
  }
  { // spawn unectas forum
    Entity *e = spawn_entity(w);
    UnitT *ut = get_unit(UFORUM, renderer, window);
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
                .t = UFORUM};

    Actionnable *ac = malloc(sizeof(Actionnable));
    *ac = (Actionnable){Lazy, UINT64_MAX};
    ecs_add_component(w, e, COMP_ACTIONNABLE, ac);
    Ownership *o = malloc(sizeof(Ownership));
    *o = (Ownership){1};
    ecs_add_component(w, e, COMP_OWNERSHIP, o);
    Position *pp = calloc(1, sizeof(Position));
    *pp = (Position){120 * 16, 40 * 16};
    ecs_add_component(w, e, COMP_POSITION, pp);
    ecs_add_component(w, e, COMP_SPRITE, u->sprite);
    ecs_add_component(w, e, COMP_UNIT, u);
    Selectable *se = calloc(1, sizeof(Selectable));
    ecs_add_component(w, e, COMP_SELECTABLE, se);
    BuildingGhost *bg = malloc(sizeof(BuildingGhost));
    *bg = (BuildingGhost){
        ut, 1000, 500, 1, UFORUM,
    };
    ecs_add_component(w, e, COMP_BUILDINGGHOST, bg);
  }
}
void revert_game(World *w) {
  // I'm not sure we actually remove everything we should here. Keep an eye open
  despawn_from_component(w, COMPF_UNIT);
  despawn_from_component(w, COMPF_BUILDINGGHOST);
  despawn_from_component(w, COMPF_MAPCOMPONENT);
  despawn_from_component(w, COMPF_PLAYERMANAGER);
  // create the players
  for (uint i = 0; i < 2; i++) {
    Entity *e = spawn_entity(w);
    PlayerManager *pm = malloc(sizeof(PlayerManager));
    *pm = (PlayerManager){i, 500, 0, 500, 0, 1.0, 1.0, 1.0, 1.0};
    ecs_add_component(w, e, COMP_PLAYERMANAGER, pm);
  }
}

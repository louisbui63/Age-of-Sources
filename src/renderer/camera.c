#include "camera.h"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdint.h>

#include "../components.h"
#include "../data_structures/asset_manager.h"
#include "../data_structures/ecs.h"
#include "../data_structures/map.h"
#include "../data_structures/vec.h"
#include "sprite.h"

Position world2screenspace(Position *p, Camera *cam) {
  return (Position){.x = (p->x - cam->x) / cam->zoom,
                    .y = (p->y - cam->y) / cam->zoom};
}

Position screen2worldspace(Position *p, Camera *cam) {
  return (Position){.x = p->x * cam->zoom + cam->x,
                    .y = p->y * cam->zoom + cam->y};
}

void render(World *w, SDL_Renderer *rdr, Camera *cam, SDL_Window *window) {

  // render map
  uint64_t mask = COMPF_MAPCOMPONENT;
  EntityRef *er = world_query(w, &mask);
  if (vec_len(er) == 1) {
    EntityRef ei = er[0];
    Entity *e = get_entity(w, ei);
    MapComponent *cmp = entity_get_component(w, e, COMP_MAPCOMPONENT);
    for (int i = 0; i < map_width(cmp->map); i++) {
      for (int j = 0; j < map_height(cmp->map); j++) {
        char *file = get_tile_file_name(cmp->map[i][j]);
        SDL_Texture *text = get_texture(file, rdr, window);

        Position p = (Position){.x = i * TILE_SIZE, .y = j * TILE_SIZE};

        Position wtl = world2screenspace(&p, cam);
        Position wtr = world2screenspace(
            &(Position){.x = p.x + TILE_SIZE, .y = p.y + TILE_SIZE}, cam);

        SDL_Rect r = {
            .x = wtl.x, .y = wtl.y, .w = wtr.x - wtl.x, .h = wtr.y - wtl.y};

        if (wtl.x < WIN_W && wtl.y < WIN_H && wtr.x > 0 && wtr.y > 0) {
          SDL_RenderCopy(rdr, text, 0, &r);
        }

        free(file);
      }
    }

  } else {
    WARN("Unexpected number of maps found");
  }

  // render sprites
  mask = COMPF_POSITION | COMPF_SPRITE;
  er = world_query(w, &mask);
  _Pragma("omp parallel") {
    _Pragma("omp for") {
      for (uint i = 0; i < vec_len(er); i++) {
        EntityRef ei = er[i];
        Entity *e = get_entity(w, ei);
        Sprite *s = entity_get_component(w, e, COMP_SPRITE);
        Position *p = entity_get_component(w, e, COMP_POSITION);
        Position tr;
        tr.x = s->rect->w - s->rect->x;
        tr.y = s->rect->h - s->rect->y;
        Position wtl = world2screenspace(p, cam);
        Position wtr = world2screenspace(
            &(Position){.x = p->x + tr.x, .y = p->y + tr.y}, cam);
        SDL_Rect r = {
            .x = wtl.x, .y = wtl.y, .w = wtr.x - wtl.x, .h = wtr.y - wtl.y};
        // occludes offscreen sprites
        if (wtl.x < WIN_W && wtl.y < WIN_H && wtr.x > 0 && wtr.y > 0) {
          _Pragma("omp critical") {
            // the documentation refuses to tell us if it is safe but as far as
            // I can tell it is (in fact, we might not even need omp critical,
            // who knows ? (not me !))
            SDL_RenderCopy(rdr, s->texture, s->rect, &r);
          }
        }
      }
    }
    _Pragma("omp barrier")
  }
}

void map_movement(World *w, SDL_Renderer *, Entity *e, Inputs *i, KeyState st) {
  int x = 0, y = 0;
  if (st == KEY_DOWN) {
    if (inputs_is_key_in(i, SDLK_DOWN))
      y++;
    if (inputs_is_key_in(i, SDLK_UP))
      y--;
    if (inputs_is_key_in(i, SDLK_RIGHT))
      x++;
    if (inputs_is_key_in(i, SDLK_LEFT))
      x--;
  }
  if (x || y) {
    Camera *c = entity_get_component(w, e, COMP_CAMERA);
    c->x += 16 * x;
    c->y += 16 * y;
    c->x = c->x < 0 ? 0
                    : (c->x > TILE_SIZE * 400 - WIN_W ? TILE_SIZE * 400 - WIN_W
                                                      : c->x);
    c->y = c->y < 0 ? 0
                    : (c->y > TILE_SIZE * 400 - WIN_H ? TILE_SIZE * 400 - WIN_H
                                                      : c->y);
  }
  if (st == KEY_PRESSED && inputs_is_key_in(i, SDLK_COMMA)) {
    Camera *c = entity_get_component(w, e, COMP_CAMERA);
    if (c->zoom == 1)
      c->zoom = 2;
    else
      c->zoom = 1;
  }
}

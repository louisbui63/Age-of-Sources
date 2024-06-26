#include "camera.h"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdint.h>

#include "../components.h"
#include "../construction.h"
#include "../data_structures/asset_manager.h"
#include "../data_structures/ecs.h"
#include "../data_structures/map.h"
#include "../data_structures/vec.h"
#include "../selection.h"
#include "anim.h"
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

  mask = COMPF_SELECTOR;
  VEC(EntityRef)
  selecteds = ((Selector *)entity_get_component(
                   w, get_entity(w, world_query(w, &mask)[0]), COMP_SELECTOR))
                  ->selected;

  SDL_Texture *bck =
      get_texture("asset/sprites/select_ally_back.bmp", rdr, window);

  // render sprites
  mask = COMPF_POSITION | COMPF_SPRITE;
  er = world_query(w, &mask);
  _Pragma("omp parallel") {
    _Pragma("omp for ordered schedule(static, 1)") for (uint i = 0;
                                                        i < vec_len(er); i++) {
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
      SDL_Rect r = {.x = wtl.x - (int)(s->rect->w / (2 * cam->zoom)),
                    .y = wtl.y - (int)(s->rect->h / (2 * cam->zoom)),
                    .w = wtr.x - wtl.x,
                    .h = wtr.y - wtl.y};
      // occludes offscreen sprites
      if (wtl.x < WIN_W && wtl.y < WIN_H && wtr.x > 0 && wtr.y > 0) {
        _Pragma("omp ordered") {
          for (uint j = 0; j < vec_len(selecteds); j++) {
            if (selecteds[j] == ei) {
              SDL_RenderCopy(rdr, bck, 0, &r);
              break;
            }
          }

          BuildingGhost *g = entity_get_component(w, e, COMP_BUILDINGGHOST);
          char is_ghost = g && !g->construction_done;
          if (is_ghost)
            SDL_SetTextureColorMod(s->texture, (Uint8)200, (Uint8)100,
                                   (Uint8)100);

          Animator *a = entity_get_component(w, e, COMP_ANIMATOR);
          char flip = (a && a->flipped) ? 1 : 0;

          // the documentation refuses to tell us if it is safe but as far
          // as I can tell it is (in fact, we might not even need omp
          // critical, who knows ? (not me !))
          SDL_RenderCopyEx(rdr, s->texture, a ? &a->current : s->rect, &r, 0, 0,
                           flip);
          if (is_ghost)
            SDL_SetTextureColorMod(s->texture, (Uint8)255, (Uint8)255,
                                   (Uint8)255);
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
                    : (c->x > TILE_SIZE * 400 - WIN_W * c->zoom
                           ? TILE_SIZE * 400 - WIN_W * c->zoom
                           : c->x);
    c->y = c->y < 0 ? 0
                    : (c->y > TILE_SIZE * 400 - (WIN_H - 90) * c->zoom
                           ? TILE_SIZE * 400 - (WIN_H - 90) * c->zoom
                           : c->y);
  }
  if (st == KEY_PRESSED && inputs_is_key_in(i, SDLK_COMMA)) {
    Camera *c = entity_get_component(w, e, COMP_CAMERA);
    if (c->zoom == 1) {
      c->zoom = 2;
      c->x = c->x < 0 ? 0
                      : (c->x > TILE_SIZE * 400 - WIN_W * c->zoom
                             ? TILE_SIZE * 400 - WIN_W * c->zoom
                             : c->x);
      c->y = c->y < 0 ? 0
                      : (c->y > TILE_SIZE * 400 - (WIN_H - 90) * c->zoom
                             ? TILE_SIZE * 400 - (WIN_H - 90) * c->zoom
                             : c->y);
    } else
      c->zoom = 1;
  }
}

#include "camera.h"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdint.h>

#include "../components.h"
#include "../data_structures/ecs.h"
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

void render(World *w, SDL_Renderer *rdr, Camera *cam) {
  uint64_t mask = COMPF_POSITION | COMPF_SPRITE;
  EntityRef *er = world_query(w, &mask);
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(w, er[i]);
    Sprite *s = entity_get_component(w, e, COMP_SPRITE);
    Position *p = entity_get_component(w, e, COMP_POSITION);

    Position tr = {.x = s->rect->w - s->rect->x, .y = s->rect->h - s->rect->y};

    Position wtl = world2screenspace(p, cam);
    Position wtr =
        world2screenspace(&(Position){.x = p->x + tr.x, .y = p->y + tr.y}, cam);

    SDL_Rect r = {
        .x = wtl.x, .y = wtl.y, .w = wtr.x - wtl.x, .h = wtr.y - wtl.y};

    if (wtl.x < WIN_W && wtl.y < WIN_H && wtr.x > 0 && wtr.y > 0) {
      SDL_RenderCopy(rdr, s->texture, s->rect, &r);
    }
  }
}

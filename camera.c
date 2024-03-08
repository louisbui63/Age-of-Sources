#include "camera.h"

#include <SDL2/SDL_render.h>
#include <stdint.h>

#include "components.h"
#include "ecs.h"
#include "vec.h"

void render(World *w, SDL_Renderer *rdr) {
  uint64_t mask = COMP_POSITION | COMP_SPRITE;
  EntityRef *er = world_query(w, &mask);
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(w, er[i]);

    // SDL_RenderCopy(rdr, test_tex, 0, 0);
  }
}

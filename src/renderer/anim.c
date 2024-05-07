#include "anim.h"

void advance_anim_state(Animator *a, AnimState as) {
  if (as != Noop) {
    const int h = a->current.h;
    a->current.y = h * as;
    a->state = as;
  }

  const int w = a->current.w;
  if (!(a->frame = (a->frame + 1) % ANIM_STEP)) {
    const int w = a->current.w;
    a->current.x += w;
  }

  if (a->current.x / w > a->max[a->state])
    a->current.x %= w;
}

Animator animator_new(Unit *unit_kind) {
  SDL_Surface *surf = SDL_LoadBMP(unit_kind->sprite_path);
  HANDLE_ERROR(!surf, SDL_GetError(), { abort(); })
}

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

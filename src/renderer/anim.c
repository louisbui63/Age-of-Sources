#include "anim.h"

void advance_anim_state(Animator *a, AnimState as, char flipped) {
  if (as != Noop) {
    const int h = a->current.h;
    a->current.y = h * as;
    a->state = as;
  }

  if (flipped != -1)
    a->flipped = flipped;

  const int w = a->current.w;
  if (!(a->frame = (a->frame + 1) % ANIM_STEP)) {
    const int w = a->current.w;
    a->current.x += w;
  }

  if (a->current.x / w > a->max[a->state])
    a->current.x %= w;
}

Animator animator_new(Unit *unit_kind) {
  // this might be the worst piece of code I have ever written, you have been
  // warned (yes, even including that time I used an excel spreadsheet to
  // interface between rust and visual basic, and no, I won't elaborate further)
  SDL_Surface *surf = SDL_LoadBMP(unit_kind->path_to_sprite);
  HANDLE_ERROR(!surf, SDL_GetError(), {
    // thankfully the os can deal with that memory leak
    abort();
  })

  int max[3];
  SDL_Rect rct = *unit_kind->sprite->rect;
  int bs = surf->format->BytesPerPixel;
  for (uint i = 0; i < 3; i++)
    for (uint j = 0; j < 3; j++) {
      uint8_t *addr =
          (uint8_t *)surf->pixels + i * rct.h * surf->pitch + j * bs * rct.w;
      uint32_t color = 0;

      for (int b = 0; b < bs; b++)
        *((uint8_t *)(&color) + b) = *addr + b;

      if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
        // Oh god no please no
        // This is untested and should remain as such
        // If you ever need this donate your 6800 to a museum and reconsider
        // your life choices instead
        uint8_t a = *(uint8_t *)(&color);
        *(uint8_t *)(&color) = *((uint8_t *)(&color) + 3);
        *((uint8_t *)(&color) + 3) = a;
        a = *((uint8_t *)(&color) + 1);
        *((uint8_t *)(&color) + 1) = *((uint8_t *)(&color) + 2);
        *((uint8_t *)(&color) + 2) = a;
      }

      SDL_Color actual_color;
      SDL_GetRGBA(color, surf->format, &actual_color.r, &actual_color.g,
                  &actual_color.b, &actual_color.a);
      if (actual_color.r == 255 && actual_color.g == 0 && actual_color.b == 0 &&
          actual_color.a == 255)
        break;
      max[i]++;
    }

  SDL_FreeSurface(surf);

  // Learn more about obscure C syntax just like this on my website at
  // uwu-segfault.eu/api/blahaj
  return (Animator){0, rct, Idle, {max[0], max[1], max[2]}, 0};
  // no, I don't intend to shitpost less
}

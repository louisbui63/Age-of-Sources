#include <stdlib.h>

#include "sprite.h"

void sprite_component_free(void *temp) {
  Sprite *spr = (Sprite *)temp;
  printf("%p\n", spr->rect);
  printf("%p\n", &spr->rect);
  if (spr->rect) {
    free(spr->rect);
  }
  free(spr);
}

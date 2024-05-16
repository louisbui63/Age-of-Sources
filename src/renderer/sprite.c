#include <stdlib.h>

#include "sprite.h"

void sprite_component_free(void *temp) {
  Sprite *spr = (Sprite *)temp;
  if (spr->rect) {
    free(spr->rect);
  }
  free(spr);
}

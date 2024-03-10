#include "components.h"

#include "camera.h"
#include "ecs.h"
#include "input.h"
#include "sprite.h"

int init_world(World *w) {
  register_system_requirement(w, COMPF_POSITION | COMPF_SPRITE);
  register_system_requirement(w, COMPF_KEY_EVENT);
  register_component(w, Position);
  register_component(w, Sprite);
  register_component(w, KeyEvent);
  return 0;
}

#include "components.h"

#include "camera.h"
#include "ecs.h"
#include "sprite.h"
#include "input.h"

int init_world(World *w) {
  register_system_requirement(w, COMP_POSITION | COMP_SPRITE);
  register_system_requirement(w, COMP_KEY_EVENT);
  register_component(w, Position);
  register_component(w, Sprite);
  register_component(w, KeyEvent);
  return 0;
}

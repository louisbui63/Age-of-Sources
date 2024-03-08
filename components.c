#include "components.h"

#include "camera.h"
#include "ecs.h"
#include "sprite.h"

int init_world(World *w) {
  register_system_requirement(w, COMP_POSITION | COMP_SPRITE);
  register_component(w, Position);
  register_component(w, Sprite);
  return 0;
}

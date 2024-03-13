#include "components.h"

#include "data_structures/ecs.h"
#include "input.h"
#include "renderer/camera.h"
#include "renderer/sprite.h"

int init_world(World *w) {
  register_system_requirement(w, COMPF_POSITION | COMPF_SPRITE);
  register_system_requirement(w, COMPF_KEY_EVENT);
  register_component(w, Position);
  register_component(w, Sprite);
  register_component(w, KeyEvent);
  return 0;
}

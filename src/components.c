#include "components.h"

#include "data_structures/ecs.h"
#include "input.h"
#include "renderer/camera.h"
#include "renderer/sprite.h"
#include "renderer/ui.h"
>>>>>>> main:src/components.c

int init_world(World *w) {
  register_system_requirement(w, COMPF_POSITION | COMPF_SPRITE);
  register_system_requirement(w, COMPF_KEY_EVENT);
  register_system_requirement(w, COMPF_BACKGROUND);
  register_system_requirement(w, COMPF_CLICKABLE);
  register_system_requirement(w, COMPF_MINIMAP);
  register_system_requirement(w, COMPF_HOVERABLE);
  register_component(w, Position);
  register_component(w, Sprite);
  register_component(w, KeyEvent);
  register_component(w, Background);
  register_component(w, Clickable);
  register_component(w, Minimap);
  register_component(w, Hoverable);
  return 0;
}

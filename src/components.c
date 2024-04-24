#include "components.h"

#include "ai/steering_behaviors.h"
#include "data_structures/ecs.h"
#include "data_structures/map.h"
#include "input.h"
#include "renderer/camera.h"
#include "renderer/sprite.h"
#include "renderer/ui.h"

int init_world(World *w) {
  register_system_requirement(w, COMPF_POSITION | COMPF_SPRITE);
  register_system_requirement(w, COMPF_POSITION | COMPF_STEERMANAGER);
  register_system_requirement(w, COMPF_POSITION | COMPF_STEERMANAGER |
                                     COMPF_STEEROBSTACLE);
  register_component(w, Position);
  register_component(w, Sprite);
  register_component(w, KeyEvent);
  register_component(w, Background);
  register_component(w, Clickable);
  register_component(w, Minimap);
  register_component(w, Hoverable);
  register_component_callback(w, MapComponent, map_component_free);
  register_component(w, SteerManager);
  register_component(w, SteerObstacle);
  return 0;
}

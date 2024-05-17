#include "components.h"

#include "ai/steering_behaviors.h"
#include "data_structures/ecs.h"
#include "data_structures/map.h"
#include "input.h"
#include "players.h"
#include "renderer/anim.h"
#include "renderer/camera.h"
#include "renderer/sprite.h"
#include "renderer/ui.h"
#include "selection.h"
#include "units/units.h"

int init_world(World *w) {
  register_system_requirement(w, COMPF_POSITION | COMPF_SPRITE);
  register_system_requirement(w, COMPF_POSITION | COMPF_STEERMANAGER);
  register_system_requirement(w, COMPF_POSITION | COMPF_STEERMANAGER |
                                     COMPF_STEEROBSTACLE);
  register_system_requirement(w, COMPF_POSITION | COMPF_STEERMANAGER |
                                     COMPF_ANIMATOR);
  register_system_requirement(w,
                              COMPF_POSITION | COMPF_SPRITE | COMPF_SELECTABLE);
  register_component(w, Position);
  register_component_callback(w, Sprite, sprite_component_free);
  register_component(w, KeyEvent);
  register_component_callback(w, Background, background_component_free);
  register_component_callback(w, Clickable, clickable_component_free);
  register_component(w, Minimap);
  register_component_callback(w, Hoverable, hoverable_component_free);
  register_component_callback(w, MapComponent, map_component_free);
  register_component(w, SteerManager);
  register_component(w, SteerObstacle);
  register_component_callback(w, Unit, unit_component_free);
  register_component_callback(w, Text, text_component_free);
  register_component(w, Animator);
  register_component(w, Camera);
  register_component(w, Selectable);
  register_component_callback(w, Selector, selector_free);
  register_component(w, PlayerManager);
  register_component_callback(w, Window, free_nothing);
  return SUCCESS;
}

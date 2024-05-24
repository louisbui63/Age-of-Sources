#include "game_manager.h"
#include "components.h"
#include "data_structures/map.h"
#include "units/units.h"

void new_game(World *w, SDL_Renderer *renderer, SDL_Window *window) {
  // generate the map from a bmp
  Entity *map = spawn_entity(w);
  MapComponent *mc = malloc(sizeof(MapComponent));
  *mc = (MapComponent){load_map_from_bmp("asset/map.bmp")};
  ecs_add_component(w, map, COMP_MAPCOMPONENT, mc);

  // spawn stuff
  spawn_unit(w, BASE_SOLDIER, renderer, window, (Position){100, 100}, 0);
  spawn_unit(w, BASE_SOLDIER, renderer, window, (Position){120, 100}, 0);
  spawn_unit(w, BASE_SOLDIER, renderer, window, (Position){100, 120}, 0);
  spawn_unit(w, BASE_FISH, renderer, window, (Position){200, 200}, 1);
}
void revert_game(World *w) {
  // I'm not sure we actually remove everything we should here. Keep an eye open
  despawn_from_component(w, COMPF_UNIT);
  despawn_from_component(w, COMPF_BUILDINGGHOST);
  despawn_from_component(w, COMPF_MAPCOMPONENT);
}

#include "construction.h"

void building_ghost_component_free(void *a) {
  BuildingGhost *bg = a;
  unit_component_free(bg->u);
  free(a);
}

//! @file construction.h
#pragma once

#include "units/units.h"

//! A component that serves as a building currently being built
typedef struct {
  Unit *u;
  int progress;
  int max;
} BuildingGhost;

void building_ghost_component_free(void *);

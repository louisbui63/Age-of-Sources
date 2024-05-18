//! @file construction.h
#pragma once

#include "data_structures/ecs.h"
#include "units/units.h"

//! A component that serves as a building currently being built
typedef struct {
  Unit *u;
  int progress;
  int max;
  char construction_done;
} BuildingGhost;

void building_ghost_component_free(void *);

//! Transforms a building's ghost into a proper building (typically, would be
//! called when it's finished being constructed)
void finish_construction(World *w, Entity *e);

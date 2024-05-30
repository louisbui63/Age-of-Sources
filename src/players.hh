//! @file players.h
#pragma once
#include "data_structures/ecs.h"

//! A component that stores the current status of one of the players
typedef struct {
  int id;
  int water;
  int dwater;
  int clay;
  int dclay;
  float damage_multiplier;
  float construct_multiplier;
  float clay_multiplier;
  float water_multiplier;
} PlayerManager;

//! A component that flags an entity as a source of water for its owner
typedef struct {
} WaterSource;

//! A component that flags an entity as a source of clay for its owner
typedef struct {
} ClaySource;

//! Self explanatory
void update_ressources(World *w);

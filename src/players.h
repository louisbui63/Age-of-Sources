//! @file players.h
#pragma once

//! A component that stores the current status of one of the players
typedef struct {
  int id;
  int water;
  int dwater;
  int clay;
  int dclay;
} PlayerManager;

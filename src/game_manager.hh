//! @file game_manager.h
#pragma once

#include "ai/ennemy_ai.h"
#include "data_structures/ecs.h"
#include "renderer/camera.h"

//! Initiates a game (i.e. creates the map, places the required units, ...)
void new_game(World *w, SDL_Renderer *renderer, SDL_Window *window, Camera *cam,
              AiState *ais);

//! Reverts any changes to the world made during a game so a new one can be
//! created
void revert_game(World *w);

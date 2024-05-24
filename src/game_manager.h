//! @file game_manager.h
#pragma once

#include "data_structures/ecs.h"
#include "renderer/camera.h"

//! initiates a game (i.e. creates the map, places the required units, ...)
void new_game(World *w, SDL_Renderer *renderer, SDL_Window *window,
              Camera *cam);

//! reverts any changes to the world made during a game so a new one can be
//! created
void revert_game(World *w);

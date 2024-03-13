//! @file asset_manager.h
#pragma once

#include <SDL2/SDL.h>

#include "hash_map.h"

//! Stores and manages the textures used in the game
// adapted from another project of mine
extern HashMap ASSET_STORE;

//! Initializes the `ASSET_STORE`; must be called before any call to
//! `get_texture` or `load_texture`
void init_asset_manager();
//! Returns a pointer to the texture from file `t`. Will had it to the
//! `ASSET_STORE` if it is not in it yet
void *get_texture(char *t, SDL_Renderer *renderer, SDL_Window *window);
//! Loads the texture from file `t` in the `ASSET_STORE`
//! While calling it multiple times with the same `t` shouldn't fail, it is
//! unadvisable as slow. Crashes on invalid file path or texture creation.
void *load_texture(char *t, SDL_Renderer *renderer, SDL_Window *window);

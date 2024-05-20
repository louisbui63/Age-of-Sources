//! @file asset_manager.h
#pragma once

#include <SDL2/SDL.h>

#include "../units/units.h"
#include "hash_map.h"

//! Stores and manages the textures used in the game
// adapted from another project of mine.
extern HashMap ASSET_STORE;

//! Initializes the `ASSET_STORE`; must be called before any call to
//! `get_texture` or `load_texture`
void init_asset_manager();

Error lock_asset(char *t, char locked);

char is_asset_locked(char *t);

//! Returns a pointer to the texture from file `t`. Will had it to the
//! `ASSET_STORE` if it is not in it yet
void *get_texture(char *t, SDL_Renderer *renderer, SDL_Window *window);

//! Loads the texture from file `t` in the `ASSET_STORE`
//! While calling it multiple times with the same `t` shouldn't fail, it is
//! unadvisable as slow. Crashes on invalid file path or texture creation.
void *load_texture(char *t, SDL_Renderer *renderer, SDL_Window *window);

//! Returns a pointer to the audio from file `t`. Will had it to the
//! `ASSET_STORE` if it is not in it yet
int drop_texture(char *t);

void *get_audio(char *t, char is_mus);

//! Loads the audio from file `t` in the `ASSET_STORE`
//! While calling it multiple times with the same `t` shouldn't fail, it is
//! unadvisable as slow. Crashes on invalid file path or audio creation.
void *load_audio(char *t, char is_mus);

int drop_audio(char *t);

void *load_font(char *t, Uint8 size);

//! Returns a pointer to the font from file `t`. Will add it to the
//! `ASSET_STORE` if it is not in it yet.
void *get_font(char *t, Uint8 size);

int drop_font(char *font, Uint8 size);

void free_asset_store();

//! @file asset_manager.h
#pragma once

#include <SDL2/SDL.h>

#include "../units/units.h"
#include "hash_map.h"

//! Stores and manages the textures used in the game
// adapted from another project of mine.
extern HashMap ASSET_STORE;

//! Initializes the `ASSET_STORE`; must be called before any call to a
//! `get_*` function or a `load_*` function.
void init_asset_manager();

//! Change the locked status of an asset to `locked`. A locked asset isn't
//! dropped by the `drop_*` functions even when it is not used anymore.
Error lock_asset(char *t, char locked);

//! Returns true iff an asset is locked. See `lock_asset` for more details
char is_asset_locked(char *t);

//! Returns a pointer to the texture from file `t`. Will add it to the
//! `ASSET_STORE` if it is not in it yet
void *get_texture(char *t, SDL_Renderer *renderer, SDL_Window *window);

//! Loads the texture from file `t` in the `ASSET_STORE`
//! While calling it multiple times with the same `t` shouldn't fail, it is
//! unadvisable as slow. Crashes on invalid file path or texture creation.
void *load_texture(char *t, SDL_Renderer *renderer, SDL_Window *window);

//! Remove a texture from the `ASSET_STORE` and free it.
int drop_texture(char *t);

//! Returns a pointer to the audio from file `t`. Will add it to the
//! `ASSET_STORE` if it is not in it yet.
void *get_audio(char *t, char is_mus);

//! Loads the audio from file `t` in the `ASSET_STORE`
//! While calling it multiple times with the same `t` shouldn't fail, it is
//! unadvisable as slow. Crashes on invalid file path or audio creation.
void *load_audio(char *t, char is_mus);

//! Frees the audio of the file `t` in the `ASSET_STORE`.
int drop_audio(char *t);

//! Loads the font from file `t` in the `ASSET_STORE` with size `size`.
//! While calling it multiple times with the same `t` shouldn't fail, it is
//! unadvisable as slow. Crashes on invalid file path or font creation.
void *load_font(char *t, Uint8 size);

//! Returns a pointer to the font from file `t`. Will add it to the
//! `ASSET_STORE` if it is not in it yet.
void *get_font(char *t, Uint8 size);

//! Remove a font from the `ASSET_STORE` and free it.
int drop_font(char *font, Uint8 size);

//! Loads the unit of the UnitTypes `t` in the `ASSET_STORE` by parsing the file
//! that  matches it. While calling it multiple times with the same `t`
//! shouldn't fail, it is unadvisable as slow. Crashes on invalid file path or
//! texture creation
void *load_unit(UnitTypes t, SDL_Renderer *renderer, SDL_Window *window);

//! Returns a pointer to the UnitT of the UnitTypes `t`. Will add it to the
//! `ASSET_STORE` if it is not in it yet.
void *get_unit(UnitTypes t, SDL_Renderer *renderer, SDL_Window *window);

//! Remove a unit from the `ASSET_STORE` and free it.
int drop_unit(UnitTypes *t);

//! Self explanatory.
void free_asset_store();

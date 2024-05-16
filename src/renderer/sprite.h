//! @file sprite.h

#pragma once
#include <SDL2/SDL_render.h>

//! A component that stores an entity's sprite
typedef struct {
  SDL_Texture *texture;
  SDL_Rect *rect;
} Sprite;

//! Frees a `Sprite`. Generally, should only be called by the ecs
void sprite_component_free(void *temp);

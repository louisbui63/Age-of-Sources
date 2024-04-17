#pragma once
#include <SDL2/SDL_render.h>

// sprites textures and rects should be managed by the asset manager, the ECS
// will make no effort to free those
typedef struct {
  SDL_Texture *texture;
  SDL_Rect *rect;
} Sprite;

void sprite_component_free(void *temp);

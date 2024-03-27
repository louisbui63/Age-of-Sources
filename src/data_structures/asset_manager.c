#include "asset_manager.h"

#include <SDL2/SDL_mixer.h>
#include <stdint.h>
#include <stdlib.h>

#include "../errors.h"
#include "../util.h"
#include "hash_map.h"

HashMap ASSET_STORE;

typedef enum { TEXTURE, SFX, MUSIC, TTF } AssetKind;

typedef struct {
  uintptr_t counter;
  void *ref;
  AssetKind kd;
} Rc;

void hmase_free(void *u) {
  HashMapEntry *w = u;
  free(w->key);
  switch (((Rc *)w->value)->kd) {

  case TEXTURE:
    SDL_DestroyTexture(((Rc *)w->value)->ref);
    break;
  case SFX:
    Mix_FreeChunk(((Rc *)w->value)->ref);
    break;
  case MUSIC:
    Mix_FreeMusic(((Rc *)w->value)->ref);
    break;
  case TTF:
    break;
  }
  free(w->value);
  free(u);
}

void free_asset_store() { hash_map_free_callback(&ASSET_STORE, hmase_free); }

void init_asset_manager() {
  ASSET_STORE = hash_map_create(hash_str, not_strcmp);
  atexit(free_asset_store);
}

void *load_texture(char *t, SDL_Renderer *renderer, SDL_Window *window) {
  SDL_Surface *surf = SDL_LoadBMP(t);
  HANDLE_ERROR(!surf, SDL_GetError(), {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    abort();
  })

  SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
  HANDLE_ERROR(!tex, SDL_GetError(), {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    abort();
  })
  SDL_FreeSurface(surf);
  char *key = malloc(strlen(t));
  strcpy(key, t);

  Rc *val = malloc(sizeof(Rc));
  *val = (Rc){.ref = tex, .counter = 0, .kd = TEXTURE};

  hash_map_insert(&ASSET_STORE, key, val);
  return tex;
}

void *get_texture(char *t, SDL_Renderer *renderer, SDL_Window *window) {
  void *tex = hash_map_get(&ASSET_STORE, t);
  if (!tex) {
    return load_texture(t, renderer, window);
  }
  ((Rc *)tex)->counter++;
  return ((Rc *)tex)->ref;
}

int drop_texture(char *t) {
  void *tex = hash_map_get(&ASSET_STORE, t);
  if (!tex) {
    return INVALID_TEXTURE;
  }
  if (!--((Rc *)tex)->counter) {
    return hash_map_delete_callback(&ASSET_STORE, t, hmase_free);
  }
  return SUCCESS;
}

void *load_audio(char *t, char is_mus) {

  void *aud;
  AssetKind kd;
  if (is_mus) {
    aud = Mix_LoadMUS(t);
    kd = MUSIC;
  } else {
    aud = Mix_LoadWAV(t);
    kd = SFX;
  }
  HANDLE_ERROR(!aud, Mix_GetError(), { abort(); });

  char *key = malloc(strlen(t));
  strcpy(key, t);

  Rc *val = malloc(sizeof(Rc));
  *val = (Rc){.ref = aud, .counter = 0, .kd = kd};

  hash_map_insert(&ASSET_STORE, key, val);
  return aud;
}

void *get_audio(char *t, char is_mus) {
  void *aud = hash_map_get(&ASSET_STORE, t);
  if (!aud) {
    return load_audio(t, is_mus);
  }
  ((Rc *)aud)->counter++;
  return ((Rc *)aud)->ref;
}

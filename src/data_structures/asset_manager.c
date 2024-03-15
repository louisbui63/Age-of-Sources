#include "asset_manager.h"

#include <stdint.h>
#include <stdlib.h>

#include "../errors.h"
#include "../util.h"
#include "hash_map.h"

HashMap ASSET_STORE;

typedef struct {
  uintptr_t counter;
  void *ref;
} Rc;

void hmase_free(void *u) {
  HashMapEntry *w = u;
  free(w->key);
  SDL_DestroyTexture(((Rc *)w->value)->ref);
  free(w->value);
  free(u);
}

void free_asset_store() { hash_map_free_callback(&ASSET_STORE, hmase_free); }

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
  *val = (Rc){.ref = tex, .counter = 0};

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

void init_asset_manager() {
  ASSET_STORE = hash_map_create(hash_str, not_strcmp);
  atexit(free_asset_store);
}

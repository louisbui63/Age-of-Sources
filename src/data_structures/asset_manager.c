#include "asset_manager.h"

#include <SDL2/SDL_ttf.h>
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

void *load_font_aux(char *t) {
  Uint8 n = strlen(t);
  Uint8 size = t[n - 1] - 32;
  t[n - 2] = 0;
  printf("%s\n%c\n",t,t[n-1]);
  printf("%d\n", size);
  TTF_Font *font = TTF_OpenFont(t, size);
  t[n - 2] = '|';
  char *key = malloc(n + 1);
  strcpy(key, t);

  Rc *val = malloc(sizeof(Rc));
  *val = (Rc){.ref = font, .counter = 0};

  hash_map_insert(&ASSET_STORE, key, val);
  return font;
}

void *load_font(char *font, Uint8 size) {
  Uint8 n = strlen(font);
  char *t = malloc(n + 3);
  strcpy(t, font);
  t[n] = '|';
  t[n + 1] = 32 + size;
  t[n + 2] = 0;
  void *f = load_font_aux(t);
  free(t);
  return f;
}

void *get_font_aux(char *t) {
  void *font = hash_map_get(&ASSET_STORE, t);
  if (!font) {
    return load_font_aux(t);
  }
  ((Rc *)font)->counter++;
  return ((Rc *)font)->ref;
}

void *get_font(char *font, Uint8 size) {
  Uint8 n = strlen(font);
  char *t = malloc(n + 3);
  strcpy(t, font);
  t[n] = '|';
  t[n + 1] = 32 + size;
  t[n + 2] = 0;
  void *f = get_font_aux(t);
  free(t);
  return f;
}

int drop_font_aux(char *t) {
  void *font = hash_map_get(&ASSET_STORE, t);
  if (!font) {
    return INVALID_FONT;
  }
  if (!--((Rc *)font)->counter) {
    return hash_map_delete_callback(&ASSET_STORE, t, hmase_free);
  }
  return SUCCESS;
}

int drop_font(char *font, Uint8 size) {
  Uint8 n = strlen(font);
  char *t = malloc(n + 3);
  strcpy(t, font);
  t[n] = '|';
  t[n + 1] = 32 + size;
  t[n + 2] = 0;
  int f = drop_font_aux(t);
  free(t);
  return f;
}

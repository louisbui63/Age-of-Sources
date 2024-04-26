#include "asset_manager.h"

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>
#include <stdlib.h>

#include "../errors.h"
#include "../util.h"
#include "hash_map.h"

HashMap ASSET_STORE;

typedef enum { TEXTURE, SFX, MUSIC, TTF } AssetKind;

//! A reference counter for the assets. Should only be used through the API
//! available in `asset_manager.h`
typedef struct {
  uintptr_t counter;
  void *ref;
  AssetKind kd;
  char locked;
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
    TTF_CloseFont(((Rc *)w->value)->ref);
    break;
  }
  free(w->value);
  free(u);
}

void free_asset_store() { hash_map_free_callback(&ASSET_STORE, hmase_free); }

void init_asset_manager() {
  ASSET_STORE = hash_map_create(hash_str, not_strcmp);
}

Error lock_asset(char *t, char locked) {
  void *at = hash_map_get(&ASSET_STORE, t);
  if (!at) {
    return ASSET_NOT_FOUND;
  }
  ((Rc *)at)->locked = locked;
  return SUCCESS;
}

char is_asset_locked(char *t) {
  void *at = hash_map_get(&ASSET_STORE, t);
  return ((Rc *)at)->locked;
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
  char *key = malloc((strlen(t) + 1) * sizeof(char));
  strcpy(key, t);

  Rc *val = malloc(sizeof(Rc));
  *val = (Rc){.ref = tex, .counter = 0, .kd = TEXTURE, .locked = 0};

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
    return ASSET_NOT_FOUND;
  }
  if (!((Rc *)tex)->locked) {
    if (!--((Rc *)tex)->counter) {
      return hash_map_delete_callback(&ASSET_STORE, t, hmase_free);
    }
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
  *val = (Rc){.ref = aud, .counter = 0, .kd = kd, .locked = 0};

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

int drop_audio(char *t) {
  void *aud = hash_map_get(&ASSET_STORE, t);
  if (!aud) {
    return ASSET_NOT_FOUND;
  }
  if (!((Rc *)aud)->locked) {
    if (!--((Rc *)aud)->counter) {
      return hash_map_delete_callback(&ASSET_STORE, t, hmase_free);
    }
  }
  return SUCCESS;
}

void *load_font_aux(char *t) {
  Uint8 n = strlen(t);
  Uint8 size = (t[n - 1] - '0') + (t[n - 2] - '0') * 10;
  t[n - 3] = 0;
  TTF_Font *font = TTF_OpenFont(t, size);
  t[n - 3] = '|';
  char *key = malloc(n + 1);
  strcpy(key, t);

  Rc *val = malloc(sizeof(Rc));
  *val = (Rc){.ref = font, .counter = 0, .kd = TTF, .locked = 0};

  hash_map_insert(&ASSET_STORE, key, val);
  return font;
}

void *load_font(char *font, Uint8 size) {
  Uint8 n = strlen(font);
  char *t = malloc(n + 4);
  strcpy(t, font);
  t[n] = '|';
  t[n + 1] = '0' + size / 10;
  t[n + 2] = '0' + size % 10;
  t[n + 3] = 0;
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
  char *t = malloc(n + 4);
  strcpy(t, font);
  t[n] = '|';
  t[n + 1] = '0' + size / 10;
  t[n + 2] = '0' + size % 10;
  t[n + 3] = 0;
  void *f = get_font_aux(t);
  free(t);
  return f;
}

int drop_font_aux(char *t) {
  void *font = hash_map_get(&ASSET_STORE, t);
  if (!font) {
    return ASSET_NOT_FOUND;
  }
  if (!((Rc *)font)->locked) {
    if (!--((Rc *)font)->counter) {
      return hash_map_delete_callback(&ASSET_STORE, t, hmase_free);
    }
  }
  return SUCCESS;
}

int drop_font(char *font, Uint8 size) {
  Uint8 n = strlen(font);
  char *t = malloc(n + 4);
  strcpy(t, font);
  t[n] = '|';
  t[n + 1] = '0' + size / 10;
  t[n + 2] = '0' + size % 10;
  t[n + 3] = 0;
  int f = drop_font_aux(t);
  free(t);
  return f;
}

#include "asset_manager.h"

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>
#include <stdlib.h>

#include "../errors.h"
#include "../parser.h"
#include "../util.h"
#include "hash_map.h"

HashMap ASSET_STORE;

typedef enum { TEXTURE, SFX, MUSIC, TTF, UNIT } AssetKind;

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
  case UNIT:
    unitt_free(((Rc *)w->value)->ref);
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
  // printf("%s\n", t);
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
    } // the sprite shouldn't be freed as it also has to be its own component
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

  char *key = malloc((strlen(t) + 1) * sizeof(char));
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

void *load_unit(UnitTypes t, SDL_Renderer *renderer, SDL_Window *window) {
  UnitT *u;
  UnitTypes *key = malloc(sizeof(UnitTypes));
  *key = t;

  switch (t) {
  case WELL:
    u = parse("src/units/unit_well.c", renderer, window);
    u->t = t;
    break;

  case FURNACE:
    u = parse("src/units/unit_furnace.c", renderer, window);
    u->t = t;
    break;

  case CASERN:
    u = parse("src/units/unit_casern.c", renderer, window);
    u->t = t;
    break;

  case TOWER:
    u = parse("src/units/unit_tower.c", renderer, window);
    u->t = t;
    break;

  case KONBINI:
    u = parse("src/units/unit_konbini.c", renderer, window);
    u->t = t;
    break;

  case HOUSE:
    u = parse("src/units/unit_house.c", renderer, window);
    u->t = t;
    break;

  case FORT:
    u = parse("src/units/unit_fort.c", renderer, window);
    u->t = t;
    break;

  case FORUM:
    u = parse("src/units/unit_forum.c", renderer, window);
    u->t = t;
    break;

  case UWELL:
    u = parse("src/units/unit_uwell.c", renderer, window);
    u->t = t;
    break;

  case UFURNACE:
    u = parse("src/units/unit_ufurnace.c", renderer, window);
    u->t = t;
    break;

  case UCASERN:
    u = parse("src/units/unit_ucasern.c", renderer, window);
    u->t = t;
    break;

  case UTOWER:
    u = parse("src/units/unit_utower.c", renderer, window);
    u->t = t;
    break;

  case UKONBINI:
    u = parse("src/units/unit_ukonbini.c", renderer, window);
    u->t = t;
    break;

  case UHOUSE:
    u = parse("src/units/unit_uhouse.c", renderer, window);
    u->t = t;
    break;

  case UFORT:
    u = parse("src/units/unit_ufort.c", renderer, window);
    u->t = t;
    break;

  case UFORUM:
    u = parse("src/units/unit_uforum.c", renderer, window);
    u->t = t;
    break;

  case BEAVER:
    u = parse("src/units/unit_beaver.c", renderer, window);
    u->t = t;
    break;

  case UBEAVER:
    u = parse("src/units/unit_ubeaver.c", renderer, window);
    u->t = t;
    break;

  case BASE_SOLDIER:
    u = parse("src/units/unit_tanuki.c", renderer, window);
    u->t = t;
    break;

  case BASE_FISH:
    u = parse("src/units/unit_fish.c", renderer, window);
    u->t = t;
    break;

  case MAID:
    u = parse("src/units/unit_maid.c", renderer, window);
    u->t = t;
    break;

  case FROG:
    u = parse("src/units/unit_frog.c", renderer, window);
    u->t = t;
    break;

  case SHRIMP:
    u = parse("src/units/unit_gun_shrimp.c", renderer, window);
    u->t = t;
    break;

  case HIPPO:
    u = parse("src/units/unit_hippo.c", renderer, window);
    u->t = t;
    break;

  case NARVAL:
    u = parse("src/units/unit_narval.c", renderer, window);
    u->t = t;
    break;

  case PINGU:
    u = parse("src/units/unit_pingu.c", renderer, window);
    u->t = t;
    break;

  case SAMURAI:
    u = parse("src/units/unit_samurai.c", renderer, window);
    u->t = t;
    break;

  case SECU:
    u = parse("src/units/unit_secu.c", renderer, window);
    u->t = t;
    break;

  case T34:
    u = parse("src/units/unit_t34.c", renderer, window);
    u->t = t;
    break;

  case DEBUG:
    u = parse("src/units/unit_debug.c", renderer, window);
    u->t = t;
    break;

  case DEBUG2:
    u = parse("src/units/unit_debug2.c", renderer, window);
    u->t = t;
    break;

  case JELLYFISH:
    u = parse("src/units/unit_jellyfish.c", renderer, window);
    u->t = t;
    break;

  default:
    u = parse("src/units/unit_tanuki.c", renderer, window);
    u->t = t;
    break;
  }

  Rc *val = malloc(sizeof(Rc));
  *val = (Rc){.ref = u, .counter = 0, .kd = UNIT, .locked = 0};

  hash_map_insert(&ASSET_STORE, key, val);

  return u;
}

void *get_unit(UnitTypes t, SDL_Renderer *renderer, SDL_Window *window) {
  void *u = hash_map_get(&ASSET_STORE, &t);
  if (!u) {
    return load_unit(t, renderer, window);
  }
  ((Rc *)u)->counter++;
  return ((Rc *)u)->ref;
}

int drop_unit(UnitTypes *t) {
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

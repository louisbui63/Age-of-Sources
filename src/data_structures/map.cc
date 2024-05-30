#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>

#include "../util.h"
#include "hash_map.h"
#include "map.h"
#include "vec.h"

Map map_create(int w, int h) {
  Map m = malloc(sizeof(int *) * w + 2 * sizeof(int));
  ((int *)m)[0] = w;
  ((int *)m)[1] = h;
  m = (Map)(((int *)m) + 2);
  for (int i = 0; i < w; i++) {
    m[i] = calloc(h, sizeof(int));
  }
  // abort();
  return m;
}
void map_free(Map m) {
  for (int i = 0; i < map_width(m); i++)
    free(m[i]);
  free((int *)m - 2);
}

void map_component_free(void *a) {
  map_free(((MapComponent *)a)->map);
  free(a);
}

char *get_tile_file_name(TileTypes tt) {
  // return should be freed !!
  int8_t id = tiletype2pixel(tt);
  char *out = malloc(sizeof(int) * strlen("asset/sprites/tileXX.bmp0"));
  uint8_t u = *(uint8_t *)&id;
  sprintf(out, "asset/sprites/tile%.2X.bmp", u);
  return out;
}

Map load_map_from_bmp(char *path) {
  SDL_Surface *surf = SDL_LoadBMP(path);
  HANDLE_ERROR(!surf, SDL_GetError(), { abort(); })

  Map out = map_create(surf->w, surf->h);

  int bs = surf->format->BytesPerPixel;
  if (bs != 1)
    abort();

  for (int i = 0; i < surf->w; i++)
    for (int j = 0; j < surf->h; j++)
      out[i][j] =
          pixel2tiletype(((int8_t *)surf->pixels)[j * surf->pitch + i * bs]);

  SDL_FreeSurface(surf);
  return out;
}

TileTypes pixel2tiletype(int8_t id) {
  uint8_t u = *(uint8_t *)&id;
  if (u == 0x00)
    return TILE_MOUNTAIN;
  if (u == 0x3D)
    return TILE_GIGEMENT;
  if (u == 0xC3)
    return TILE_WATER;
  if (u == 0xD8)
    return TILE_SWAMP;
  if (u == 0xFF)
    return TILE_PLAIN;
  return -1;
}

int8_t tiletype2pixel_wrapped(TileTypes id) {
  if (id == TILE_MOUNTAIN)
    return 0x00;
  if (id == TILE_GIGEMENT)
    return 0x3D;
  if (id == TILE_WATER)
    return 0xC3;
  if (id == TILE_SWAMP)
    return 0xD8;
  if (id == TILE_PLAIN)
    return 0xFF;
  return -1;
}

int8_t tiletype2pixel(TileTypes id) {
  int8_t i = tiletype2pixel_wrapped(id);
  return *(int8_t *)&i;
}

#include <SDL2/SDL.h>
#include <stdint.h>
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
  m = (Map)((int *)(m) + 2);
  for (int i = 0; i < w; i++)
    m[i] = calloc(h, sizeof(int));
  return m;
}

void map_free(Map m) {
  for (int i = 0; i < map_width(m); i++)
    free(m[i]);
  free((int *)m - 2);
}

char *get_tile_file_name(int8_t id) {
  // return should be freed !!
  char *out = malloc(sizeof(int) * strlen("asset/tileXX.png0"));
  uint8_t u = *(uint8_t *)&id;
  sprintf(out, "asset/tile%.2X.png", u);
  return out;
}

Map load_map_from_bmp(char *path) {
  SDL_Surface *surf = SDL_LoadBMP(path);
  HANDLE_ERROR(!surf, SDL_GetError(), { abort(); })

  Map out = map_create(surf->w, surf->h);

  int bs = surf->format->BytesPerPixel;
  if (bs != 1)
    abort();

  int row = 0, column = 0;
  for (int8_t *i = surf->pixels;
       i != (int8_t *)surf->pixels + surf->h * surf->w; i++) {
    out[row][column] = *i;
    column = (column + 1) % surf->w;
    if (!column)
      row++;
  }

  SDL_FreeSurface(surf);
  return out;
}

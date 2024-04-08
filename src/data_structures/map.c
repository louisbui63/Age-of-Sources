#include <stdlib.h>

#include "map.h"

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

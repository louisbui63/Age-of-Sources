#include "pathfinding.h"

#include <stdio.h>

#include "../src/data_structures/vec.h"
#include "../src/errors.h"
#include "../src/pathfinding.h"
#include "../src/util.h"

void print_pathln(Path p) {
  printf("[");
  for (uint i = 0; i < vec_len(p) - 1; i++) {
    printf("{x=%d,y=%d}, ", p[i]->x, p[i]->y);
  }
  if (vec_len(p) > 0)
    printf("{x=%d,y=%d}", p[vec_len(p) - 1]->x, p[vec_len(p) - 1]->y);
  printf("]\n");
}

int test_pathfinding() {
  Map m = map_create(10, 10);

  // testing (0,0) -> (0,0)
  TilePosition src = {.x = 0, .y = 0};
  TilePosition dest = {.x = 0, .y = 0};
  Path p = pathfind_astar(m, UNIT_TEST, &src, &dest);
  ASSERT(vec_len(p) == 1);
  path_free(p);

  // testing diagonal
  src = (TilePosition){.x = 0, .y = 0};
  dest = (TilePosition){.x = 1, .y = 1};
  p = pathfind_astar(m, UNIT_TEST, &src, &dest);
  ASSERT(p != 0);
  ASSERT(vec_len(p) == 2);
  path_free(p);

  src = (TilePosition){.x = 2, .y = 0};
  dest = (TilePosition){.x = 1, .y = 1};
  p = pathfind_astar(m, UNIT_TEST, &src, &dest);
  ASSERT(p != 0);
  ASSERT(vec_len(p) == 2);
  path_free(p);

  src = (TilePosition){.x = 0, .y = 0};
  dest = (TilePosition){.x = 7, .y = 3};
  p = pathfind_astar(m, UNIT_TEST, &src, &dest);
  ASSERT(p != 0);
  ASSERT(vec_len(p) == 8);
  path_free(p);

  m[1][1] = TILE_FOREST;
  m[1][0] = TILE_FOREST;
  m[6][3] = TILE_FOREST;
  src = (TilePosition){.x = 0, .y = 0};
  dest = (TilePosition){.x = 7, .y = 3};
  p = pathfind_astar(m, UNIT_TEST, &src, &dest);
  ASSERT(p != 0);
  ASSERT(vec_len(p) == 9);
  path_free(p);

  map_free(m);
  return SUCCESS;
}

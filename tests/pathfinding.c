#include "pathfinding.h"

#include "../src/data_structures/vec.h"
#include "../src/errors.h"
#include "../src/pathfinding.h"
#include "../src/util.h"

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

  map_free(m);
  return SUCCESS;
}

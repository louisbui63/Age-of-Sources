#include "pathfinding.h"

#include "../src/pathfinding.h"
#include "../src/util.h"
#include "../src/errors.h"
#include "../src/data_structures/vec.h"

int test_pathfinding(){
  Map m = map_create(10,10);
  TilePosition src = {.x=0, .y=0};
  TilePosition dest = {.x=0, .y=0};
  Path p = pathfind_astar(m,UNIT_TEST,&src,&dest);
  ASSERT(vec_len(p) == 1);
  return SUCCESS;
}

#include "vec2.h"

#include "../src/util.h"

int test_vec2() {
  Vec2 v1 = {.x=0,.y=0};
  Vec2 v2 = {.x=0,.y=0};
  Vec2 r = v2add(v1,v2);
  ASSERT(r.x==0 && r.y==0);
  v1.x=3.4;v1.y=7;
  v2.x=-1.1;v2.y=4.5;
  r = v2add(v1,v2);
  ASSERT(r.x==2.3 && r.y==11.5);
  return SUCCESS;
}

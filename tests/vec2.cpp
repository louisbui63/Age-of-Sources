#include "vec2.hpp"

#include "../src/util.hpp"

int test_vec2() {
  Vec2 v1 = {.x = 0, .y = 0};
  Vec2 v2 = {.x = 0, .y = 0};
  Vec2 r = v1 + v2;
  ASSERT(r.x == 0 && r.y == 0);
  v1.x = 3;
  v1.y = 7;
  v2.x = -1;
  v2.y = 4;
  r = v1 + v2;
  ASSERT(r.x == 2 && r.y == 11);

  r = v1 - v2;
  ASSERT(r.x == 4 && r.y == 3);

  r = 2 * v1;
  ASSERT(r.x == 6 && r.y == 14);

  r = v1 * 2;
  ASSERT(r.x == 6 && r.y == 14);

  Vec2 v3 = {.x = 10, .y = 0};
  r = v3.normalize();
  ASSERT(r.x == 1 && r.y == 0);

  v3.y = 10;
  v3.x = 0;
  float f = v3.angle();
  ASSERT(f == 0);

  f = v3.len();
  ASSERT(f == 10);

  r = v3.truncate(4);
  ASSERT(r.x == 0 && r.y == 4);

  r = v3 / 2;
  ASSERT(r.x == 0 && r.y == 5);

  f = v1 * v2;
  ASSERT(f == 25);

  return SUCCESS;
}

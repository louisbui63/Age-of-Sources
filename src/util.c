#include "util.h"

#include <math.h>
#include <string.h>
#include <time.h>

void free_nothing(void *) {}

char not_strcmp(void *a, void *b) {
  return !(char)strcmp((char *)a, (char *)b);
}

void sleep_nano(uint64_t n) {
  struct timespec rem;
  rem.tv_nsec = n % 1'000'000'000;
  rem.tv_sec = n / 1'000'000'000;

  int r;
  r = nanosleep(&rem, &rem);
  while (r) {
    r = nanosleep(&rem, &rem);
  }
}

#define v2op(op, name)                                                         \
  Vec2 v2##name(Vec2 a, Vec2 b) { return (Vec2){a.x op b.x, a.y op b.y}; }

v2op(-, sub)
v2op(+, add)
Vec2 v2mul(float a, Vec2 b) { return (Vec2){a * b.x, a * b.y}; }
Vec2 v2normalize(Vec2 a) {
  if (a.x == 0 && a.y == 0)
    return a;
  float len = sqrtf(a.x * a.x + a.y * a.y);
  return (Vec2){a.x / len, a.y / len};
}
float v2angle(Vec2 a) { return atan2f(a.x, a.y); }
float v2len(Vec2 a) { return sqrtf(a.x * a.x + a.y * a.y); }

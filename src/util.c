#include "util.h"
#include "components.h"

#include <math.h>
#include <string.h>
#include <time.h>

void free_nothing(__attribute__((unused)) void *_) {}

char not_strcmp(void *a, void *b) {
  return !(char)strcmp((char *)a, (char *)b);
}

void sleep_nano(uint64_t n) {
  struct timespec rem;
  rem.tv_nsec = n % 1000000000;
  rem.tv_sec = n / 1000000000;

  int r;
  r = nanosleep(&rem, &rem);
  while (r) {
    r = nanosleep(&rem, &rem);
  }
}

#define v2op(op, name)                                                         \
  inline Vec2 v2##name(Vec2 a, Vec2 b) {                                       \
    return (Vec2){a.x op b.x, a.y op b.y};                                     \
  }

v2op(-, sub)
v2op(+, add)
inline Vec2 v2mul(float a, Vec2 b) { return (Vec2){a * b.x, a * b.y}; }
Vec2 v2normalize(Vec2 a) {
  if (a.x == 0 && a.y == 0)
    return a;
  float len = sqrtf(a.x * a.x + a.y * a.y);
  return (Vec2){a.x / len, a.y / len};
}
inline float v2angle(Vec2 a) { return atan2f(a.x, a.y); }
inline float v2len(Vec2 a) { return sqrtf(a.x * a.x + a.y * a.y); }
inline Vec2 v2truncate(Vec2 a, float b) {
  return (v2len(a) > b) ? v2mul(b, v2normalize(a)) : a;
}
inline Vec2 v2div(Vec2 a, float b) { return (Vec2){a.x / b, a.y / b}; }
inline float v2dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }

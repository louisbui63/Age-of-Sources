#include "util.h"

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

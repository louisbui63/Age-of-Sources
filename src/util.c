#include "util.h"

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

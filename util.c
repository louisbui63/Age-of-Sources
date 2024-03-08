#include "util.h"

#include <string.h>

void free_nothing(__attribute__((unused)) void *_) {}

char not_strcmp(void *a, void *b) {
  return !(char)strcmp((char *)a, (char *)b);
}

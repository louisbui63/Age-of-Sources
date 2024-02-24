#pragma once

#include <stdio.h>
#define HANDLE_ERROR(err, message, callback)                                   \
  {                                                                            \
    if (err) {                                                                 \
      fprintf(stderr, "[%s:%d] %s\n", __FILE__, __LINE__, message);            \
      callback;                                                                \
    }                                                                          \
  }

#define TARGET_FRAMETIME (1000 / 60)

#define ASSERT(a)                                                              \
  {                                                                            \
    if (!(a)) {                                                                \
      fprintf(stderr, "[%s:%d] assertion failed\n", __FILE__, __LINE__);       \
      return -1;                                                               \
    }                                                                          \
  }

void free_nothing(void *);
char not_strcmp(void *a, void *b);

typedef unsigned int uint;

#define max(a, b) ((a > b) ? (a) : (b))
#define min(a, b) ((a < b) ? (a) : (b))

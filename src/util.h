#pragma once

#include <stdint.h>
#include <stdio.h>
#include <time.h>

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
      fprintf(stderr, "[%s:%d] assertion '%s' failed\n", __FILE__, __LINE__,   \
              #a);                                                             \
      return -1;                                                               \
    }                                                                          \
  }

#define TIME(label, commands)                                                  \
  {                                                                            \
    struct timespec _beg;                                                      \
    clock_gettime(CLOCK_MONOTONIC, &_beg);                                     \
    commands;                                                                  \
    struct timespec _end;                                                      \
    clock_gettime(CLOCK_MONOTONIC, &_end);                                     \
    uint64_t elapsed = (uint64_t)(_end.tv_sec - _beg.tv_sec) * 1000000000 +    \
                       (uint64_t)(_end.tv_nsec - _beg.tv_nsec);                \
    printf("[\033[38;5;96mBENCHMARKING\033[0m] %s took %fs\n", label,          \
           (double)elapsed / 1000000000);                                      \
  }

void free_nothing(void *);
char not_strcmp(void *a, void *b);
void sleep_nano(uint64_t n);

typedef unsigned int uint;

// typedef struct {
//   uint left;
//   uint right;
// } UintPair;

#define max(a, b) ((a > b) ? (a) : (b))
#define min(a, b) ((a < b) ? (a) : (b))

//! @file util.h
#pragma once

#include "errors.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>

//! prints `message` as a warning
#define WARN(message)                                                          \
  fprintf(stderr, "[%s:%d] [\x1b[33mW\x1b[0m] %s\n", __FILE__, __LINE__,       \
          message);

//! prints `message` when `err != 0`, and then runs `callback`
#define HANDLE_ERROR(err, message, callback)                                   \
  {                                                                            \
    if (err) {                                                                 \
      fprintf(stderr, "[%s:%d] [\x1b[31mE\x1b[0m] %s\n", __FILE__, __LINE__,   \
              message);                                                        \
      callback;                                                                \
    }                                                                          \
  }

//! The frametime that the game should try to maintain, in milliseconds
#define TARGET_FRAMETIME (1000 / 60)

//! Verify that `a != 0`. Otherwise, prints an error and exits the current
//! function with error `-1`
#define ASSERT(a)                                                              \
  {                                                                            \
    if (!(a)) {                                                                \
      fprintf(stderr, "[%s:%d] [\x1b[31mE\x1b[0m] assertion '%s' failed\n",    \
              __FILE__, __LINE__, #a);                                         \
      return ASSERTION_FAILED;                                                 \
    }                                                                          \
  }

//! Benchmarks `commands`
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

//! Does nothing. Used when a callback is necessary but nothing is to be done
void free_nothing(void *);
//! Strictly equivalent to `!strcmp(a,b)`. Used as a callback
char not_strcmp(void *a, void *b);
//! Sleeps the calling thread for n nanoseconds. Uses GNU extensions
void sleep_nano(uint64_t n);

typedef unsigned int uint;

#define max(a, b) ((a > b) ? (a) : (b))
#define min(a, b) ((a < b) ? (a) : (b))

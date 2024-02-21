#pragma once

#define HANDLE_ERROR(err, message, callback)                                   \
  {                                                                            \
    if (err) {                                                                 \
      fprintf(stderr, "[%s:%d]%s", __FILE__, __LINE__, message);               \
      callback;                                                                \
    }                                                                          \
  }

#define TARGET_FRAMETIME (1000 / 60)

void free_nothing(void *) {}

#define max(a, b) ((a > b) ? (a) : (b))
#define min(a, b) ((a < b) ? (a) : (b))

#pragma once

#define HANDLE_ERROR(err, callback)                                            \
  {                                                                            \
    if (err < 0) {                                                             \
      fprintf(stderr, "%s", SDL_GetError());                                   \
      callback;                                                                \
    }                                                                          \
  }

#define HANDLE_NULLPTR(err, callback)                                          \
  {                                                                            \
    if (!err) {                                                                \
      fprintf(stderr, "%s", SDL_GetError());                                   \
      callback;                                                                \
    }                                                                          \
  }

#define TARGET_FRAMETIME (1000 / 60)

#define max(a, b) ((a > b) ? (a) : (b))
#define min(a, b) ((a < b) ? (a) : (b))

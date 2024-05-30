//! @file util.hh
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#include "errors.hh"
#include <cstdint>
#include <cstdio>
#include <ctime>

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
#define TARGET_FRAMETIME (1000.0 / 60.0)

//! The main window's logical height
#define WIN_H 360
//! The main window's logical width
#define WIN_W 640

//! Verify that `a != 0`. Otherwise, prints an error and exits the current
//! function with error `ASSERTION_FAILED`
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

//! Self explanatory.
#define max(a, b) ((a > b) ? (a) : (b))

//! Self explanatory.
#define min(a, b) ((a < b) ? (a) : (b))

//! A 2d vector used for the units'movement
struct Vec2 {
  float x;
  float y;

  //! substracts two `Vec2`
  Vec2 operator-(Vec2);
  //! adds two `Vec2`
  Vec2 operator+(Vec2);
  //! performs a product between `Vec2` `a` and the scalar `b`
  Vec2 operator*(float);
  //! performs a dot product between two `Vec2`
  float operator*(Vec2);
  //! performs a product between `Vec2` `a` and the scalar `1/b`
  Vec2 operator/(float);
  //! returns the angle (in radian) between `a` and the `(0,1)` vector
  float angle();
  //! returns the length of a `Vec2`
  float len();
  //! returns a vector of same direction than `a` and of length `max(v2len(a),
  //! b)`
  Vec2 truncate(float);
  //! normalizes a `Vec2`
  Vec2 normalize();
};

//! This enum is used to know the state of the game.
typedef enum {
  STOP,
  MAIN,
  OPTIONMAIN,
  IN_GAME,
  IN_GAMEMENU,
  IN_GAMEOPTION,
  VICTORY,
  DEFEAT
} Running;

//! This is a type that was created to be able to get the window from the world
//! using the ecs.
typedef struct {
  SDL_Window *w;
} Window;

//! This is a type that was created to be able to get the renderer from the
//! world using the ecs.
typedef struct {
  SDL_Renderer *r;
} Renderer;

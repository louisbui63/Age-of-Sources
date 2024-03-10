#include "ecs.h"
#include <SDL2/SDL_render.h>

#define WIN_H 360
#define WIN_W 640

//! The `Camera` struct is not a component, it is meant to have exactly one
//! instance and serves as the base for screenspace<->worldspace calculations
typedef struct {
  float x;
  float y;
  //! `zoom` is such that if `zoom==1`, one pixel in screenspace is one pixel in
  //! worldspace, while if `zoom==2`, one pixel in screenspace is two pixels in
  //! worldspace
  float zoom;
} Camera;

typedef struct {
  float x;
  float y;
} Position;

void render(World *w, SDL_Renderer *rdr, Camera *cam);

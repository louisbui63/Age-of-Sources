//! @file camera.h
#include "../data_structures/ecs.h"
#include <SDL2/SDL_render.h>

//! The main window's height
#define WIN_H 360
//! The main window's width
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

//! A component that contains the world space coordinates of an entity
typedef struct {
  float x;
  float y;
} Position;

//! Transfers `p` to screenspace, according to `cam`
Position world2screenspace(Position *p, Camera *cam);
//! Transfers `p` to worldspace, according to `cam`
Position screen2worldspace(Position *p, Camera *cam);

//! Renders any entity with a `Position` and a `Sprite`, according to `cam`.
//! Said position must be in worldspace coordinates. Also renders the map if
//! found.
void render(World *w, SDL_Renderer *rdr, Camera *cam, SDL_Window *window);

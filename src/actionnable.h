//! @file actionnable.h

#include "data_structures/ecs.h"

//! the action performed by a unit
typedef enum {
  //! The unit does nothing
  Lazy,
  //! The unit is building something
  Build,
  //! The unit is attacking something
  Attack,
  //! The unit is producing something
  Produce,
} Action;

//! A component that holds a unit's current action, i.e. what it's
//! attacking/building
typedef struct {
  Action act;
  EntityRef target;
} Actionnable;

//! Performs the action specified in `ac` on the target. Returns 1 if an action
//! is being performed, 0 otherwise
char actionnate(World *w, Actionnable *ac, Entity *se);

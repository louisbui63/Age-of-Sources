#include "ecs.h"

#define COMP_POSITION (1 << 0)
#define COMP_POSITION_INDEX 0

#define COMP_SPRITE (1 << 1)
#define COMP_SPRITE_INDEX 1

#define COMP_KEY_EVENT (1 << 2)
#define COMP_KEY_EVENT_INDEX 2

int init_world(World *w);

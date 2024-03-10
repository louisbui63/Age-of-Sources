#include "ecs.h"

#define COMPF_POSITION (1 << 0)
#define COMP_POSITION (0)
#define COMPF_SPRITE (1 << 1)
#define COMP_SPRITE (1)
#define COMPF_KEY_EVENT (1 << 2)
#define COMP_KEY_EVENT 2

int init_world(World *w);

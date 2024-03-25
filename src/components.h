#include "data_structures/ecs.h"

#define COMPF_POSITION (1 << 0)
#define COMP_POSITION 0
#define COMPF_SPRITE (1 << 1)
#define COMP_SPRITE 1
#define COMPF_KEY_EVENT (1 << 2)
#define COMP_KEY_EVENT 2
#define COMPF_BACKGROUND (1 << 3)
#define COMP_BACKGROUND 3
#define COMPF_CLICKABLE (1 << 4)
#define COMP_CLICKABLE 4
#define COMPF_BUTTON (1 << 5)
#define COMP_BUTTON 5
#define COMPF_MINIMAP (1 << 6)
#define COMP_MINIMAP 6
#define COMPF_HOVERABLE (1 << 7)
#define COMP_HOVERABLE 7

int init_world(World *w);

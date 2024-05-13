#pragma once

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
#define COMPF_MINIMAP (1 << 5)
#define COMP_MINIMAP 5
#define COMPF_HOVERABLE (1 << 6)
#define COMP_HOVERABLE 6
#define COMPF_MAPCOMPONENT (1 << 7)
#define COMP_MAPCOMPONENT 7
#define COMPF_STEERMANAGER (1 << 8)
#define COMP_STEERMANAGER 8
#define COMPF_STEEROBSTACLE (1 << 9)
#define COMP_STEEROBSTACLE 9
#define COMPF_UNIT (1 << 10)
#define COMP_UNIT 10
#define COMPF_TEXT (1 << 11)
#define COMP_TEXT 11
#define COMPF_MAP_DEPTH (1 << 12)
#define COMP_MAP_DEPTH 12
#define COMPF_UNIT_DEPTH (1 << 13)
#define COMP_UNIT_DEPTH 13
#define COMPF_STATIC_UI_DEPTH (1 << 14)
#define COMP_STATIC_UI_DEPTH 14
#define COMPF_INTERACTIVE_UI_DEPTH (1 << 15)
#define COMP_INTERACTIVE_UI_DEPTH 15

int init_world(World *w);

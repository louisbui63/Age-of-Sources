#include "ecs.h"

#include "../ecs.h"
#include "../util.h"
#include <stdint.h>

typedef struct {
  int y;
  double uwu;
} TestComp;

int test_ecs() {
  World w = world_new();

  ASSERT(!register_component(&w, TestComp));
  ASSERT(!register_component(&w, float));
  ASSERT(!register_component(&w, uint8_t));
  ASSERT(!register_component(&w, int16_t));

  register_system_requirement(&w, 0x9);

  for (int i = 0; i < 48; i++) {
    Entity *e = spawn_entity(&w);
    ASSERT(e);
    TestComp *t = malloc(sizeof(TestComp));
    TestComp tmp = {i, (double)i / 2};
    *t = tmp;
    ecs_add_component(&w, e, 0, t);
    uint8_t *u = malloc(sizeof(uint8_t));
    *u = i + 1;
    ecs_add_component(&w, e, 2, u);
  }

  world_free(&w);
  return 0;
}

#include "ecs.h"

#include "../src/data_structures/ecs.h"
#include "../src/data_structures/vec.h"
#include "../src/util.h"
#include <stdint.h>
#include <stdlib.h>

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

  Bitflag c = 0x5;
  Bitflag d = 0x2;

  register_system_requirement(&w, c);

  for (int i = 0; i < 32; i++) {
    Entity *e = spawn_entity(&w);
    ASSERT(e);
    TestComp *t = malloc(sizeof(TestComp));
    TestComp tmp = {i, (double)i / 2};
    *t = tmp;
    ecs_add_component(&w, e, 0, t);
    uint8_t *u = malloc(sizeof(uint8_t));
    *u = i + 1;
    ecs_add_component(&w, e, 2, u);
    if (i < 15) {
      float *v = malloc(sizeof(float));
      *v = (float)i;
      ecs_add_component(&w, e, 1, v);
    }
  }

  register_system_requirement(&w, d);
  uint32_t ok = 0;

  EntityRef *er = world_query(&w, &c);
  ASSERT(er);
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(&w, er[i]);
    int predicted = -1;
    TestComp *c1 = entity_get_component(&w, e, 0);
    ASSERT(c1);
    ASSERT(c1->y == 2 * c1->uwu);
    if (predicted == -1)
      predicted = c1->y;
    ASSERT(predicted == c1->y)
    uint8_t *c2 = entity_get_component(&w, e, 2);
    ASSERT(c2);
    ASSERT(c1->y == 2 * c1->uwu);
    if (predicted == -1)
      predicted = *c2 - 1;
    ASSERT(predicted != (int)*c2)
    ASSERT(predicted != -1 && predicted < 32);
    ok |= 1 << predicted;
  }
  ASSERT(ok == 0xffffffff);

  uint32_t u = 0;
  EntityRef **err = world_query_mut(&w, &d);
  ASSERT(err);
  ASSERT(vec_len(*err) == 15);
  while (vec_len(*err) > 0) {
    Entity *e = get_entity(&w, *err[0]);
    u |= 1 << ((TestComp *)entity_get_component(&w, e, 0))->y;
    despawn_entity(&w, e);
  }

  er = world_query(&w, &c);
  ASSERT(er);
  ASSERT(vec_len(er) == 17);
  for (uint i = 0; i < vec_len(er); i++) {
    Entity *e = get_entity(&w, er[i]);
    u |= 1 << ((TestComp *)entity_get_component(&w, e, 0))->y;
  }

  ASSERT(u == 0xffffffff);

  float *uwu = malloc(sizeof(float));
  Entity *e = spawn_entity(&w);
  ecs_add_component(&w, e, 0, uwu);

  world_free(&w);
  return 0;
}

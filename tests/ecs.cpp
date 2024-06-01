#include "ecs.hpp"

#include "../src/data_structures/ecs.hpp"
#include "../src/util.hpp"
#include <cstdint>
#include <cstdlib>

typedef struct {
  int y;
  double uwu;
} TestComp;

int test_ecs() {
  World w = World();

  ASSERT(!w.register_component<TestComp>());
  ASSERT(!w.register_component<float>());
  ASSERT(!w.register_component<uint8_t>());
  ASSERT(!w.register_component<int16_t>());

  Bitflag c = 0b101;
  Bitflag d = 0b010;

  w.register_system_requirement(c);

  for (int i = 0; i < 32; i++) {
    Entity *e = w.spawn_entity();
    ASSERT(e);

    TestComp *t = new TestComp;
    *t = {i, (double)i / 2};
    w.add_component(e, 0, t);
    uint8_t *u = new uint8_t;
    *u = i + 1;
    w.add_component(e, 2, u);
    if (i < 15) {
      float *v = new float;
      *v = (float)i;
      w.add_component(e, 1, v);
    }
  }

  w.register_system_requirement(d);
  uint32_t ok = 0;

  std::vector<EntityRef> er = w.query(c);
  ASSERT(er.size());
  for (uint i = 0; i < er.size(); i++) {
    Entity *e = w.get_entity(er[i]);
    int predicted = -1;
    TestComp *c1 = w.get_component<TestComp *>(e, 0);
    ASSERT(c1);
    ASSERT(c1->y == 2 * c1->uwu);
    if (predicted == -1)
      predicted = c1->y;
    ASSERT(predicted == c1->y)
    uint8_t *c2 = w.get_component<uint8_t *>(e, 2);
    ASSERT(c2);
    ASSERT(c1->y == 2 * c1->uwu);
    if (predicted == -1)
      predicted = *c2 - 1;
    ASSERT(predicted != (int)*c2)
    ASSERT(predicted != -1 && predicted < 32);
    ok |= 1 << predicted;
  }
  ASSERT(ok == 0xffffffff);

  ok = 0;
  TIME("parallel query", {
    parallelize_query(er, {
      sleep_nano(100'000'000);
      Entity *e = w.get_entity(ei);
      TestComp *c = w.get_component<TestComp *>(e, 0);
      ok |= 1 << c->y;
    });
  });
  ASSERT(ok = 0xffffffff);

  ok = 0;
  TIME("sequential query", {
    for (uint i = 0; i < er.size(); i++) {
      sleep_nano(100'000'000);
      Entity *e = w.get_entity(er[i]);
      TestComp *c = w.get_component<TestComp *>(e, 0);
      ok |= 1 << c->y;
    }
  });
  ASSERT(ok = 0xffffffff);

  uint32_t u = 0;
  std::vector<EntityRef> err = w.query(d);
  ASSERT(err.size() == 15);
  while (err.size() > 0) {
    Entity *e = w.get_entity(err[0]);
    u |= 1 << w.get_component<TestComp *>(e, 0)->y;
    w.despawn_entity(e);
    err = w.query(d);
  }

  er = w.query(c);
  ASSERT(er.size() == 17);
  for (uint i = 0; i < er.size(); i++) {
    Entity *e = w.get_entity(er[i]);
    u |= 1 << w.get_component<TestComp *>(e, 0)->y;
  }

  ASSERT(u == 0xffffffff);

  float *uwu = new float;
  Entity *e = w.spawn_entity();
  w.add_component(e, 0, uwu);

  return SUCCESS;
}

#include <stdlib.h>
#include <math.h>

#include "pqueue.h"

#include "../src/data_structures/pqueue.h"
#include "../src/util.h"

typedef struct {
  int x;
  int y;
} Test;

int test_pqueue() {
  // pqueue_new and pqueue_len tests
  PQueue p = pqueue_new();
  ASSERT(pqueue_len(p) == 0);

  // pqueue_push, pqueue_get and pqueue_pop tests
  Test t1 = {17, 60};
  Test t2 = {-40, 0};
  Test t3 = {34, 11};
  Test t4 = {79, 69};

  pqueue_push(p, &t1, 2.6);
  pqueue_push(p, &t2, 4.7);
  pqueue_push(p, &t3, 2.5);
  pqueue_push(p, &t4, 7.1);
  ASSERT(pqueue_len(p)==4);

  ASSERT(pqueue_get(p)->value == &t3);
  ASSERT(pqueue_pop(p)->weight == 2.5);

  ASSERT(pqueue_get(p)->value == &t1);
  ASSERT(pqueue_len(p)==3);

  pqueue_free(p);

  // pqueue_free_callback test
  p = pqueue_new();

  Test* a1 = malloc(sizeof(Test));
  Test* a2 = malloc(sizeof(Test));
  Test* a3 = malloc(sizeof(Test));

  pqueue_push(p, a1, -36);
  pqueue_push(p, a2, -INFINITY);
  pqueue_push(p, a3, 0.0);

  ASSERT(pqueue_get(p)->value == a2);

  pqueue_free_callback(p,free);

  return SUCCESS;
}

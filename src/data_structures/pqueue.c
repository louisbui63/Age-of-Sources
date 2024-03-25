#include <stdlib.h>

#include "../util.h"
#include "pqueue.h"

#define pqueue_parent(i) ((i) / 2)
#define pqueue_left(i) ((i)*2)
#define pqueue_right(i) ((i)*2 + 1)

void swap(PQueue p, uint i, uint j) {
  PQueueEntry *pe = p[i];
  p[i] = p[j];
  p[j] = pe;
}

int min_child(PQueue p, uint i) {
  uint l = pqueue_left(i);
  uint r = pqueue_right(i);
  return p[l]->weight > p[r]->weight ? r : l;
}

void percolate_down(PQueue p, uint i) {
  uint l = pqueue_left(i);
  uint r = pqueue_right(i);
  if (l >= pqueue_len(p))
    return;
  if (r >= pqueue_len(p)) {
    if (p[l]->weight < p[i]->weight)
      swap(p, i, l);
    return;
  }
  if (p[l]->weight < p[i]->weight || p[r]->weight < p[i]->weight) {
    int m = min_child(p, i);
    swap(p, i, m);
    percolate_down(p, m);
  }
}

void percolate_up(PQueue p, uint i) {
  if (i == 0)
    return;
  if (p[pqueue_parent(i)]->weight < p[i]->weight)
    return;
  swap(p, i, pqueue_parent(i));
  percolate_up(p, pqueue_parent(i));
}

void pqueue_free(PQueue p) { pqueue_free_callback(p, free_nothing); }

void pqueue_free_callback(PQueue p, void (*callback)(void *)) {
  while (pqueue_len(p)) {
    PQueueEntry *pe = pqueue_get(p);
    callback(pe->value);
    free(pe);
    pqueue_pop(p);
  }
  vec_free(p);
}

PQueueEntry *pqueue_pop(PQueue p) {
  PQueueEntry *pe = p[0];
  swap(p, 0, pqueue_len(p) - 1);
  vec_pop(p);
  percolate_down(p, 0);
  return pe;
}

PQueueEntry *pqueue_get(PQueue p) { return p[0]; }

void pqueue_push(PQueue p, void *val, double weight) {
  PQueueEntry *pe = malloc(sizeof(PQueueEntry));
  *pe = (PQueueEntry){.value = val, .weight = weight};
  vec_push(p, pe);
  percolate_up(p, pqueue_len(p) - 1);
}

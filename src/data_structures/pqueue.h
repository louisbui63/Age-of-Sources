//! @file pqueue.h
#pragma once

#include "vec.h"

//! an entry within a `PQueue`
typedef struct {
  void *value;
  double weight;
} PQueueEntry;

//! a priority queue
typedef VEC(PQueueEntry *) PQueue;

//! creates an empty `PQueue`
#define pqueue_new() vec_new(PQueueEntry *)

//! returns the number of elements currently into the queue
#define pqueue_len(p) vec_len(p)

//! frees the queue (does not free the elements still within the queue)
void pqueue_free(PQueue p);

//! frees the queue and call `callback` on each element still in the queue
void pqueue_free_callback(PQueue p, void (*callback)(void *));

//! removes and returns the element with the smallest weight from the queue
PQueueEntry *pqueue_pop(PQueue p);

//! returns the element with the smallest weight in the queue
PQueueEntry *pqueue_get(PQueue p);

//! puts an element in the queue
void pqueue_push(PQueue p, void *val, double weight);
